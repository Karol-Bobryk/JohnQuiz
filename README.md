# Programisterzy

**Treść zadania:**

>Gra w stylu teleturnieju „Milionerzy”. Gracz udziela odpowiedzi na kolejno zadawane pytania. Do każdego
pytania jest kilka opcji odpowiedzi do wyboru. Kwota wygranej wzrasta w przypadku wyboru poprawnej
odpowiedzi na pytanie. Do dyspozycji mamy kilka kół ratunkowych (np. 50 na 50, publiczność, telefon do
przyjaciela). Pytania są odczytywane z pliku tekstowego. Program powinien umożliwiać dodawanie
nowych pytań oraz zapis ich do pliku.


## Opis programu

Program składa się z następujących plików:

- `main.c` - główny plik źródłowy, zawiera funkcję main() będącą punktem wejścia programu

- `quiz.c` - plik ten odpowiada za logikę gry

- `quiz.h` - plik nagłówkowy zawierający deklaracje zmiennych, funkcji i struktur używanych w quiz.c

- `gui.c` - plik ten odpowiada za wyświetlanie interfejsu dla użytkownika

- `gui.h` - plik nagłówkowy zawierający deklaracje zmiennych, funkcji i struktur używanych w gui.c

- `questions.txt` - w tym pliku zawarte są pytania zapisane w następujący sposób:

        Treść pytania:Odpowiedź 1:Odpowiedź 2:Odpowiedź 3:Odpowiedź 4:Indeks poprawnej odpowiedzi (od 0 do 3):Tresc telefonu do przyjaciela

- `title.txt` - plik ten zawiera napis "Programisterzy" zapisany za pomocą znaków ASCII

## Opis poszczególnych funkcji

1. **main.c**

    - `int main()`
        - funkcja ta inicjalizuje stan gry i rozpoczyna pętle gry

2. **quiz.c**

    - `#define DEFAULT_FILE "questions.txt"`
        - deklaracja pliku zawierającego pytania

    - `GameState* GameStateInit()` 
        - inicjalizuje strukture gamestate.
        - zwracana wartość: 
            - funkcja zwraca zainicjalizowaną strukturę

    - `void GameStateReset(GameState *gs)`
        - resetuje wszystkie wartości zawarte w strukturze GameState.
        - przyjmowane argumenty: 
            - struktura gamestate

    - `void GameStateFree(Gamestate* gs)` 
        - zwalnia strukture GameState
        - przyjmowane argumenty:
            - struktura gamestate

    - `size_t fGetSegLen(FILE *f)` 
        - liczy długość tekstu ograniczonego dwukropkiem lub znakiem nowej lini
        - przyjmowane argumenty:
            - wskaźnik do pliku zawierającego segment
        - zwracana wartość:
            - długość segmentu
    
    - `fDecodeQuestion(FILE *qf, Question* q, Lifelines* ll)`
        - czyta zawartość pytania i odpowiedzi i umieszcza je w strukturze Question
        - przyjmowane argumenty:
            - qf - plik zawierajacy pytania
            - q - referencja do struktury Question
            - ll - referencja do struktury Lifelines
        - zwracana wartość:
            - 0 - funkcja wykonała się bez błędu
            - -1 - funkcja wykonała się z błędem

    - `void printLifelines(Lifelines* ll)`
        - wyświetla zawartość struktury Lifelines
        - przyjmowane argumenty:
            - wskaźnik do struktury Lifelines

    - `void printQuestion(Question* q)`
        - wyświetla zawartość struktury Lifelines
        - przyjmowane argumenty:
            - wskaźnik do struktury Lifelines

    - `void printGameState(GameState* gs)`
        - wyświetla zawartosć struktury GameState
        - przyjmowane argumenty:
            - wskaźnik do struktury GameState

    - `void freeDecodedQuestion(Question* q, Lifelines* ll)`
        - zwalnia pamięć zajętą przez struktury Question i Lifelines
        - przyjmowane argumenty:
            - q - referencja do struktury Question
            - ll - referencja do struktury Lifelines

    - `#define MAX_QUESTION_SIZE 256`
        - maksymalna długość treści pytania
    
    - `int fAppendQuestion(FILE* f)`
        - funkcja ta przyjmuje wartości od użytkownika i formatuje je w sposób taki aby program mógł pytanie potem odczytać i zapisuje pytanie do pliku.
        - przyjmowane argumenty:
            - wskaźnik do pliku
        - zwracana wartość:
            - 0 - funkcja wykonała się bez błędu
            - -1 - funkcja wykonała się z błędem

    - `void strTrimNewLine(char* sBuf)`
        - usuwa znak nowej linii na końcu ciągu znaków jeśli taki występuje
        - przyjmowane argumenty:
            - ciąg znaków

    - `size_t fCountLines(FILE* file)`
        - liczy liczbe znaków nowej linii w pliku
        - przyjmowane argumenty:
            - wskaźnik do pliku
        - zwracana wartość:
            - liczba nowych linii

    - `#define QUESTION_BUFFER_SIZE (MAX_QUESTION_SIZE * 6 + 2 + 1)`
        - maksymalna długość linijki zawierająca dane do pytania

    - `int fGetRandomQuestion(GameState* gs)`
        - wybiera losowe pytanie z pliku z pytaniami i umieszcza je w strukturze Question
        - przyjmowane argumenty:
            - struktura GameState
        - zwracana wartość:
            - 0 - funkcja wykonała się bez błędu

    - `size_t getRandomQuestionId(size_t blacklist[15], size_t curId, size_t lineCount)`
        - wybiera losowe pytanie (które jeszcze nie wystąpiło) i umieszcza je na liście pytań które nie powinny już się ponownie pojawić
        - przyjmowane argumenty:
            - blacklist - list indeksów pytań które były już wykorzystane
            - curId - indeks obecnego pytania
            - lineCount - liczba linii w pliku z pytaniami
        - zwracana wartość:
            - ID następnego pytania

    - `int mainGameLoop(GameState *gs)`
        - obsługuje pętle główną gry - wybiera i wyświetla pytania, ustawia wartości nagród
        - przyjmowane argumenty:
            - gs - wskaźnik do struktury GameState
        - zwracana wartość:
            - 0 - po wyświetleniu ekranu endGame
    
    - `#define ENABLED gs->lifelines.enabledAnswers`
        - tablica zawierająca informacje o aktywnych kołach ratunkowych

    - `bool handleQuestionInput(GameState* gs)`
        - obsługa interakcji z użytkownikiem np. wybieranie pytań lub kół ratunkowych
        - przyjmowane argumenty:
            - gs - wskaźnik do struktury GameState
        - zwracana wartość:
            - true/false - true jeśli wybór użytkownika jest zgodny z założeniami programu, false jeśli tak nie jest

    - `PrizeLL* PrizeLLInit()`
        - inicjalizuje linked liste z nagrodami
        - zwracana wartość:
            - element listy

    - `void PrizeLLFree(PrizeLL* pLL)`
        - zwalnia pamięć elementu listy
        - przyjmowane argumenty:
            - element listy PrizeLL

3. **quiz.h: opis struktur**

    - `Lifelines`
        - struktura zawierająca informacje o kołach ratunkowych, składająca się z:
            - `is50_50InUse` - `boolean` określający, czy **50/50** jest w tej chwili używane.
            - `isAudienceHelpInUse` - `boolean` określający, czy **pomoc widowni** jest w tej chwili używana.
            - `isPhoneFriendInUse` - `boolean` określający, czy **telefon do przyjaciela** jest w tej chwili używany.

            - `is50_50Used` - `boolean` określający, czy zużyte zostało **50/50**.
            - `isAudienceHelpUsed` - `boolean` określający, czy zużyta została **pomoc widowni**.
            - `isPhoneFriendUsed` - `boolean` określający, czy zużyty został **telefon do przyjaciela**.
        
            - `phoneFriendContent` i `phoneFriendContentLen` - ciąg znaków zawierający wiadomość pomocniczą wypisaną po użyciu **telefonu do przyjaciela**, oraz jego długość.
            - `enabledAnswers[2]` - lista zawierająca dwie pozostałe odpowiedzi, które zostają wybrane po użyciu **50/50**.
            - `answBars[4]` - lista zawierająca długosć pasków wyświetlanych po użyciu **pomocy widowni**.
        

    - `Question`
        - struktura pytania, składająca się z:
            - `curId` - ID obecnego pytania
            - `strContent` i `strContentLen` - zawartość i długość ciągu znaków zawierającego pytanie.
            - `answ[4]` - lista zawierająca 4 możliwe odpowiedzi.
            - `answLen[4]` - lista zawierająca długości ciągów znakowych w liscie `answ`
            - `correctAnsw` - index poprawnej odpowiedzi w liscie `answ`

    - `GameState`
        - struktura, na której opiera się cała gra. Zawiera ona w sobie informacje o stanie gry, takie jak:
            - `prizeSecured` - nagroda, jaką mamy już gwarantowaną.
            - `prizeNext` - nagroda, o jaką będziemy grali w następnej rundzie.
            - `prizeCur` - nagroda, o jaką gramy teraz.

            - `*questionsFile` - wskaźnik do pliku zawierającego pytania.
            - `questionsFileLineCount` - ilość linii (czyli pytań) w pliku.
            - `questionsIdBlacklist[15]` - lista wyłączonych, które są do niej dopisywane po wybraniu dobrej odpowiedzi, aby nie uzyskiwać duplikatów.
            - `question` - obecne pytanie typu `Question`
            - `lifelines` - struktura `Lifelines` dla obecnej gry.

    - `PrizeLL`
        - struktura zawierająca informacje o wygranej kwocie
            - `isSecure` - informacja czy jest to nagroda gwarantowana
            - `value` - wartość wygranej
            - `next` - wskaźnik na następny element w liście

4. **gui.c**
    - `int getWindowWidth()`
        - liczy ilosc kolumn w okienku konsolowym.
        - **zwracana wartosc**: ilosc kolumn w okienku konsolowym.
    
    - `#define TITLE_FILE "title.txt"`
        - deklaracja pliku, z ktorego czytamy grafikę ASCII wyswietlana w funkcji `drawTitle()`

    - `int drawTitle()`
        - wyswietla wycentrowany ekran tytulowy (z pliku title.txt)
        - **zwracana wartosc**: 
            - `0`, jeśli poprawnie się wykona
            - `1`, jeśli wystąpi błąd.
    
    - `void drawMenu()`
        - wyswietla opcje menu na ekranie tytulowym.

    - `void getMenuChoice(GameState *gs)`
        - obsluguje wykonanie wyboru przez uzytkownika opcji w menu.
        - **przyjmowane argumenty**: struktura gamestate.
    
    - `void printPadding(size_t padding)`
        - wypisuje podana ilosc nowych linii, które służą jako padding.
        - **przyjmowane argumenty**: ilosc linii do wypisania.

    - `void printCenteredText(const char* textContent, int padding)`
        - wypisuje podany ciąg znaków wraz z odpowiednią ilością paddingu tak, aby był wycentrowany.
        - **przyjmowane argumenty**:
            - napis do wycentrowania,
            - ilość nowych linii

    - `void showAboutGameScreen()`
        - pokazuje ekran 'O grze'
    
    - Linie 212-224:
        - mamy tutaj 10 instrukcji `#define`, które służą jako określenie koloru i tła tekstu, który jest później używany w wyświetlaniu interfejsu.
    
    - `void printAudienceHelp(GameState *gs)`
        - wyświetla 'wykres' słupkowy, pokazujący opinię widowni odnośnie poprawnego pytania.
        - **przyjmowane argumenty**: struktura gamestate.
    
    - `void printSimpleGameGui(GameState *gs, SimpleGuiSelectedItem selectedItem, bool isConfirmed)`
        - wyświetla interfejs graficzny pokazujący stan obecnej rundy, pytania etc.
        - interfejs graficzny jest w dwóch stanach: zatwierdzony, i niezatwierdzony, w zależności od tego, czy użytkownik potwierdził wybór pytania.
        - **przyjmowane argumenty**:
            - struktura gamestate
            - wybrana opcja menu,
            - stan interfejsu

    - `void showGameOverScreen()`
        - wyswietla ekran przegranej.
        - **przyjmowane argumenty**: struktura gamestate

    - `void showVictoryScreen()`:
        - wyswietla ekran przegranej.
        - **przyjmowane argumenty**: struktura gamestate

5. **gui.h: opis struktur**

    - `SimpleGuiSelectedItem`
        - struktura zawierająca w sobie opcje, które wybrać może użytkownik podczas grania w grę:
            - `Ans{A-D}` - Odpowiedzi A-D
            - `LL50_50` - Koło ratunkowe (LL - Lifeline) 50/50
            - `LLAudHelp` - Koło ratunkowe - pomoc widowni
            - `LLPhone` - Koło ratunkowe - telefon do przyjaciela.

    

