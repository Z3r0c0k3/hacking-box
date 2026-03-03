#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= 설정 및 상수 정의 =================
// LCD 설정 (주소는 보통 0x27 또는 0x3F 입니다)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 부저 핀 정의 (PWM 핀 권장)
#define BUZZER_PIN 7

#define START_STR "aegis"
#define NUM_PROBLEMS 4
#define LINE_SPACING 2
#define LONG_SPACING 17
#define VERY_LONG_SPACING 26

// 음계 주파수 정의 (멜로디용)
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_AS6 1865
#define NOTE_C7 2093

// 문제별 정답
const char *ANSWERS[NUM_PROBLEMS] = {
    "Aegis{4}",
    "Aegis{D0_Y0u_Kn0W_Wh@T_3nC0dInG_1s?}",
    "Aegis{CQCQCQDEAEGISAEGISAR}",
    "Aegis{C0ngr@tU1AtI0nS_Y0u_h@cK3D_@11_Th3s3_d3vIc3s}"};

// EEPROM 주소 정의
const int eepromAddresses[NUM_PROBLEMS] = {0, 1, 2, 3};

// ================= PROGMEM 문자열 =================
const char intro[] PROGMEM =
    "====================================================================================================================\n"
    "                    /$$$$$$$  /$$   /$$ /$$   /$$        /$$$$$$                      /$$          \n"
    "                   | $$__  $$| $$  /$$/| $$  | $$       /$$__  $$                    |__/          \n"
    "                   | $$  \\ $$| $$ /$$/ | $$  | $$      | $$  \\ $$  /$$$$$$   /$$$$$$  /$$  /$$$$$$$\n"
    "                   | $$  | $$| $$$$$/  | $$  | $$      | $$$$$$$$ /$$__  $$ /$$__  $$| $$ /$$_____/\n"
    "                   | $$  | $$| $$  $$  | $$  | $$      | $$__  $$| $$$$$$$$| $$  \\ $$| $$|  $$$$$$ \n"
    "                   | $$  | $$| $$\\  $$ | $$  | $$      | $$  | $$| $$_____/| $$  | $$| $$ \\____  $$\n"
    "                   | $$$$$$$/| $$ \\  $$|  $$$$$$/      | $$  | $$|  $$$$$$$|  $$$$$$$| $$ /$$$$$$$/\n"
    "                   |_______/ |__/  \\__/ \\______/       |__/  |__/ \\_______/ \\____  $$|__/|_______/ \n"
    "                                                                            /$$  \\ $$              \n"
    "                                                                           |  $$$$$$/              \n"
    "                                                                            \\______/               \n"
    "====================================================================================================================\n"
    "                         \" HELLO, WELCOME TO HACKING BOX IN Dankook University Aegis. \"\n"
    "====================================================================================================================\n"
    "                                게임 방법을 모르겠다면, 'help'를 입력하고 엔터를 입력해주세요!\n"
    "\n"
    "                                      문제 1  :  The Word\n"
    "                                      문제 2  :  CQ CQ CQ\n"
    "                                      문제 3  :  Decoding This Word\n"
    "                                      문제 4  :  King Carsar\n\n"
    "                                      clear  :  시리얼 모니터의 모든 내용이 사라집니다\n"
    "                                      help   :  게임 설명과 FLAG 형식이 들어있습니다\n"
    "                                      status :  어떤 문제를 풀이했는지 보여줍니다.\n"
    "===================================================================================================================\n";

const char gameRule[] PROGMEM =
    " ---------------------------------------------------<< 게임 방법 >>---------------------------------------------------\n"
    "                               1. 풀고자 하는 문제의 번호를 입력한다. (1~4번 순서대로 풀어주세요!)\n"
    "                               2. 출제된 문제를 풀이하고 FLAG 형식에 맞춰 정답을 입력한다.\n"
    "                               3. 정답을 맞춘다면 LCD 화면의 상태가 O로 바뀌고 알림음이 울립니다!\n"
    "                               4. 게임 진입 후, 나가고 싶다면 문제 실행 후 \'exit\'를 입력한다.\n"
    "====================================================================================================================\n"
    "                                            FLAG 형식은 \"Aegis{...}\" 입니다.\n"
    "====================================================================================================================\n";

const char p3[] PROGMEM =
    "====================================================================================================================\n"
    " ------------------------------------------<  문제 03. Decoding This Word  >---------------------------------------- \n"
    "|                                                                                                                    |\n"
    "|                    아래 읽을 수 없는 문자열이 있어요. 해커한테 물어보니, 알아볼 수 없게 문자 인코딩을 바꿔놨다고 해요.                   |\n"
    "|                            문자열을 읽거나 알아볼 수 있게 변환해야 해요! 어떻게 하면 변환할 수 있을까요?                            |\n"
    "|                                                                                                                    |\n"
    " -------------------------------------------------<  인코딩 된 문자열  >----------------------------------------------- \n"
    "|                                 QWVnaXN7RDBfWTB1X0tuMFdfV2hAVF8zbkMwZEluR18xcz99                                  |\n"
    " ------------------------------------------------------------------------------------------------------------------ \n"
    "====================================================================================================================\n";

const char p2[] PROGMEM =
    "====================================================================================================================\n"
    " -----------------------------------------------<  문제 02. CQ CQ CQ  >--------------------------------------------- \n"
    "|                                                                                                                    |\n"
    "|                          아래 보면 점과 선으로 이루어진 문자열이 있어요. 통신에서 사용하는 부호라고 하던데...                          |\n"
    "|                         아래 문자열을 알아볼 수 있는 평문으로 변환해서 FLAG 형식에 맞춰 정답을 입력해주세요!                         |\n"
    "|                                                                                                                    |\n"
    " -----------------------------------<  Aegis{...} <- 중괄호 안에 답을 넣고 입력해주세요  >--------------------------------- \n"
    "|                     -.-. --.- -.-. --.- -.-. --.- -.. . .- . --. .. ... .- . --. .. ... .- .-.                    |\n"
    " ------------------------------------------------------------------------------------------------------------------ \n"
    "====================================================================================================================\n";

const char p1[] PROGMEM =
    "====================================================================================================================\n"
    " -----------------------------------------------<  문제 01. The Word  >--------------------------------------------- \n"
    "|                                                                                                                   |\n"
    "|                             Aegis는 단국대학교에 전통 깊은 개발/보안 중앙 동아리로 알려져 있습니다.                               |\n"
    "|                                     그러면 단국대 Aegis 중앙동아리는 어떻게 발음할까요?                                     |\n"
    "|                                                                                                                    |\n"
    " -----------------------------------<  Aegis{...} <- 중괄호 안에 번호를 넣고 입력해주세요  >-------------------------------- \n"
    "|                              1. Aegis[아이기스]                         2. Aegis[에이기스]                            |\n"
    "|                              3. Aegis[에이지스]                         4. Aegis[이지스]                              |\n"
    " ------------------------------------------------------------------------------------------------------------------ \n"
    "====================================================================================================================\n";

const char p4[] PROGMEM =
    "====================================================================================================================\n"
    " ----------------------------------------------<  문제 04. King Caesar  >------------------------------------------- \n"
    "|                                                                                                                   |\n"
    "|       예전에 로마 제국에 카이사르 라는 왕이 살았어요. 카이사르 왕은 동맹군과 소통하는것을 좋아했는데 특히 애용하던 암호화 알고리즘이 있어요.      |\n"
    "|   아래 문자열은 카이사르 왕이 애용한 암호화 알고리즘으로 암호화한 암호문이에요. 사용된 암호화 알고리즘을 찾고 취약점을 공격해서 FLAG를 얻으세요!   |\n"
    "|                                                                                                                   |\n"
    " -------------------------------------------------<  암호화 된 암호문  >----------------------------------------------- \n"
    "|                                Dhjlv{F0qju@wX1DwL0qV_B0x_k@fN3G_@11_Wk3v3_g3yLf3v}                               |\n"
    " ------------------------------------------------------------------------------------------------------------------ \n"
    "====================================================================================================================\n";

const char allSolve[] PROGMEM =
    "         /$$   /$$                     /$$                       /$$       /$$$$$$$$ /$$           /$$           /$$                       /$$ /$$        \n"
    "        | $$  | $$                    | $$                      | $$      | $$_____/|__/          |__/          | $$                      | $$| $$        \n"
    "        | $$  | $$  /$$$$$$   /$$$$$$$| $$   /$$  /$$$$$$   /$$$$$$$      | $$       /$$ /$$$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$   /$$$$$$$| $$        \n"
    " /$$$$$$| $$$$$$$$ |____  $$ /$$_____/| $$  /$$/ /$$__  $$ /$$__  $$      | $$$$$   | $$| $$__  $$| $$ /$$_____/| $$__  $$ /$$__  $$ /$$__  $$| $$ /$$$$$$\n"
    "|______/| $$__  $$  /$$$$$$$| $$      | $$$$$$/ | $$$$$$$$| $$  | $$      | $$__/   | $$| $$  \\ $$| $$|  $$$$$$ | $$  \\ $$| $$$$$$$$| $$  | $$|__/|______/\n"
    "        | $$  | $$ /$$__  $$| $$      | $$_  $$ | $$_____/| $$  | $$      | $$      | $$| $$  | $$| $$ \\____  $$| $$  | $$| $$_____/| $$  | $$            \n"
    "        | $$  | $$|  $$$$$$$|  $$$$$$$| $$ \\  $$|  $$$$$$$|  $$$$$$$      | $$      | $$| $$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$|  $$$$$$$ /$$        \n"
    "        |__/  |__/ \\_______/ \\_______/|__/  \\__/ \\_______/ \\_______/      |__/      |__/|__/  |__/|__/|_______/ |__/  |__/ \\_______/ \\_______/|__/\n"
    "                                                                                                                                                   \n"
    "     /$$      /$$ /$$                    /$$                            /$$$$$$                                    /$$             /$$               /$$\n"
    "    | $$$    /$$$|__/                   |__/                           /$$__  $$                                  | $$            | $$              | $$\n"
    "    | $$$$  /$$$$ /$$  /$$$$$$$ /$$$$$$$ /$$  /$$$$$$  /$$$$$$$       | $$  \\__/  /$$$$$$  /$$$$$$/$$$$   /$$$$$$ | $$  /$$$$$$  /$$$$$$    /$$$$$$ | $$\n"
    "    | $$ $$/$$ $$| $$ /$$_____//$$_____/| $$ /$$__  $$| $$__  $$      | $$       /$$__  $$| $$_  $$_  $$ /$$__  $$| $$ /$$__  $$|_  $$_/   /$$__  $$| $$\n"
    "    | $$  $$$| $$| $$|  $$$$$$|  $$$$$$ | $$| $$  \\ $$| $$  \\ $$      | $$      | $$  \\ $$| $$ \\ $$ \\ $$| $$  \\ $$| $$| $$$$$$$$  | $$    | $$$$$$$$|__/\n"
    "    | $$\\  $ | $$| $$ \\____  $$\\____  $$| $$| $$  | $$| $$  | $$      | $$    $$| $$  | $$| $$ | $$ | $$| $$  | $$| $$| $$_____/  | $$ /$$| $$_____/    \n"
    "    | $$ \\/  | $$| $$ /$$$$$$$//$$$$$$$/| $$|  $$$$$$/| $$  | $$      |  $$$$$$/|  $$$$$$/| $$ | $$ | $$| $$$$$$$/| $$|  $$$$$$$  |  $$$$/|  $$$$$$$ /$$\n"
    "    |__/     |__/|__/|_______/|_______/ |__/ \\______/ |__/  |__/       \\______/  \\______/ |__/ |__/ |__/| $$____/ |__/ \\_______/   \\___/   \\_______/|__/\n"
    "                                                                                                        | $$                                            \n"
    "                                                                                                        | $$                                            \n"
    "                                                                                                        |__/                                            \n"
    "===========================================================================================================================================================\n"
    "                                                     DKU Aegis Hacking Badge을 성공적으로 해킹하였습니다!\n"
    "                                                                        축하합니다!\n"
    "                                          You successfully hacked the DKU Aegis Hacking Badge! Congratulations!\n"
    "===========================================================================================================================================================\n";

const char *const problemTexts[NUM_PROBLEMS] PROGMEM = {p1, p2, p3, p4};

// ================= 함수 정의 =================

// EEPROM 초기화
void resetEEPROM()
{
    for (int i = 0; i < NUM_PROBLEMS; i++)
    {
        EEPROM.update(eepromAddresses[i], 0);
    }
}

// EEPROM 상태 읽기
int statusEEPROM(int idx)
{
    return EEPROM.read(eepromAddresses[idx]) == 1 ? 1 : 0;
}

// 합계 계산
int sumEEPROM()
{
    int sum = 0;
    for (int i = 0; i < NUM_PROBLEMS; i++)
    {
        sum += statusEEPROM(i);
    }
    return sum;
}

// 모든 문제 해결 여부 (수정: 1개만 풀어도 All Clear 로직으로 넘어감)
bool allProblemsCompleted()
{
    for (int i = 0; i < NUM_PROBLEMS; i++)
    {
        if (statusEEPROM(i)) // 하나라도 1(성공)인 상태라면
            return true;     // 바로 true(전체 완료)를 반환
    }
    return false; // 모두 0일 때만 false 반환
}

// PROGMEM 출력
void printProgmemString(const char *progmemString)
{
    for (int i = 0; i < strlen_P(progmemString); i++)
    {
        char c = pgm_read_byte(progmemString + i);
        Serial.print(c);
    }
}

// 빈 줄 출력
void printEmptyLines(int count)
{
    for (int i = 0; i < count; i++)
    {
        Serial.println();
    }
}

// ================= 부저 및 LCD 제어 함수 =================

// 문제 진입 효과음 (짧은 비프)
void playEntrySound()
{
    tone(BUZZER_PIN, NOTE_C6, 100);
    delay(150);
    noTone(BUZZER_PIN);
}

// 정답 효과음 (띠링!)
void playCorrectSound()
{
    tone(BUZZER_PIN, NOTE_E5, 100);
    delay(100);
    tone(BUZZER_PIN, NOTE_G5, 150);
    delay(150);
    noTone(BUZZER_PIN);
}

// 오답 효과음 (삑-)
void playWrongSound()
{
    tone(BUZZER_PIN, NOTE_C4, 200);
    delay(200);
    noTone(BUZZER_PIN);
}

// 슈퍼 마리오 스테이지 클리어 멜로디
void playMarioClearSound()
{
    int melody[] = {
        NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_E6,
        NOTE_G6, NOTE_E6, 
        NOTE_GS4, NOTE_C5, NOTE_DS5, NOTE_GS5, NOTE_C6, NOTE_DS6,
        NOTE_GS6, NOTE_DS6,
        NOTE_AS4, NOTE_D5, NOTE_F5, NOTE_AS5, NOTE_D6, NOTE_F6,
        NOTE_AS6, NOTE_AS6, NOTE_AS6, NOTE_C7
    };
    
    int durations[] = {
        100, 100, 100, 100, 100, 100, 
        300, 300, 
        100, 100, 100, 100, 100, 100, 
        300, 300,
        100, 100, 100, 100, 100, 100, 
        200, 200, 200, 1000
    };

    int length = sizeof(melody) / sizeof(int);

    for (int i = 0; i < length; i++)
    {
        tone(BUZZER_PIN, melody[i], durations[i]);
        delay(durations[i] * 1.1); // 음 사이 약간의 간격
    }
    noTone(BUZZER_PIN);
}

// LCD에 현재 상태 업데이트 (메인 화면)
void updateStatusLCD()
{
    lcd.clear();
    
    // [변경] 모든 문제 해결 시, 성공 화면을 고정으로 표시
    if (allProblemsCompleted())
    {
        lcd.setCursor(0, 0);
        lcd.print("HACKING SUCCESS!");
        lcd.setCursor(0, 1);
        lcd.print("You are root XD");
    }
    else
    {
        // 아직 다 못 풀었을 때는 기존 O/X 현황 표시
        lcd.setCursor(1, 0);
        lcd.print("Aegis Hack Box");

        lcd.setCursor(0, 1);
        for (int i = 0; i < NUM_PROBLEMS; i++)
        {
            lcd.print(i + 1);
            lcd.print(":");
            if (statusEEPROM(i))
            {
                lcd.print("O ");
            }
            else
            {
                lcd.print("X ");
            }
        }
    }
}

// Serial에만 축하 메시지 출력
void showCompletionMessageSerial()
{
    printProgmemString(allSolve);
    printEmptyLines(LINE_SPACING);
}

// 문제 풀이 로직
void handleProblem(int problemNumber)
{
    int index = problemNumber - 1;

    // 문제 진입 LCD 및 효과음
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No. ");
    lcd.print(problemNumber);
    lcd.print(" solving...");
    playEntrySound();

    printEmptyLines(LINE_SPACING);
    printProgmemString((const char *)pgm_read_word(&problemTexts[index]));
    printEmptyLines(LONG_SPACING);

    while (true)
    {
        String answer = Serial.readStringUntil('\n');
        answer.trim();
        if (answer.equals(""))
            continue;

        if (answer.equals(ANSWERS[index]))
        {
            // 정답 처리 및 EEPROM 저장
            EEPROM.update(eepromAddresses[index], 1);
            
            // =========================================================
            // [변경] 실행 순서: 1. LCD 출력 -> 2. 사운드 -> 3. Serial
            // =========================================================

            if (allProblemsCompleted()) 
            {
                // --- Case 1: 모든 문제 해결 (All Clear) ---
                
                // 1. LCD 출력 (updateStatusLCD가 성공 메시지 출력함)
                updateStatusLCD(); 
                
                // 2. 사운드 출력 (마리오 멜로디)
                playMarioClearSound();

                // 3. Serial 출력
                Serial.println("정답입니다!! 정답 갯수:" + String(sumEEPROM()) + "/4");
                printEmptyLines(LINE_SPACING);
                showCompletionMessageSerial();
            }
            else 
            {
                // --- Case 2: 개별 문제 해결 (아직 남음) ---

                // 1. LCD 출력
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("No.");
                lcd.print(problemNumber);
                lcd.setCursor(0, 1);
                lcd.print("Solved :)");

                // 2. 사운드 출력
                playCorrectSound();

                // 3. Serial 출력
                Serial.println("정답입니다!! 정답 갯수:" + String(sumEEPROM()) +
                           "/4\n\'aegis\'를 입력해서 처음으로 돌아가주세요.");
                printEmptyLines(VERY_LONG_SPACING);
                
                // 잠시 후 상태 화면 복귀
                delay(1500); 
                updateStatusLCD();
            }
            break;
        }
        else if (answer.equals("exit"))
        {
            // 나가기 처리
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Exiting");
            lcd.setCursor(0, 1);
            lcd.print("Problem...");
            
            Serial.println("\n게임에서 나가기를 선택했습니다. \'aegis\'를 입력해서 처음으로 돌아가주세요.");
            delay(1000);
            
            // 메인 상태 화면으로 복귀
            updateStatusLCD();
            
            printEmptyLines(VERY_LONG_SPACING);
            break;
        }
        else
        {
            // 오답 처리
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("No.");
            lcd.print(problemNumber);
            lcd.setCursor(0, 1);
            lcd.print("Retry :(");
            
            playWrongSound();
            
            Serial.println("입력값: " + answer);
            Serial.println("정답이 아니거나 FLAG 형식에 맞지 않습니다. 다시 시도하세요.");
            
            delay(1000);
            
            // 1초 후 다시 solving 메시지로 복귀
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("No. ");
            lcd.print(problemNumber);
            lcd.print(" solving...");
        }
    }
}

void setup()
{
    Serial.begin(4800);
    pinMode(BUZZER_PIN, OUTPUT);

    // LCD 초기화
    lcd.init();
    lcd.backlight();
    
    // ================== [추가된 부분] 부팅 시퀀스 ==================
    // 1. Initializing + Loading Bar
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");

    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) {
        lcd.write(0xFF); // 꽉 찬 네모 칸(Block Character) 출력
        delay(100);      // 속도 조절 (전체 약 1.6초)
    }
    delay(500); // 100% 완료 후 잠시 대기

    // 2. Version Screen
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aegis Hack Box");
    lcd.setCursor(0, 1);
    lcd.print("Version 1.0.0");
    delay(1500); // 1.5초간 유지
    // ============================================================

    // 시작 시 LCD 상태 표시
    updateStatusLCD();

    // 시작 알림음
    playCorrectSound();

    if (Serial)
    {
        printEmptyLines(LINE_SPACING);
        Serial.println("게임을 시작합니다. \'help\'를 입력하여 도움말을 확인하세요.");
        delay(1000);
        printProgmemString(intro);
        printEmptyLines(LINE_SPACING);
        if (allProblemsCompleted())
        {
            // 이미 다 깬 상태라면 Serial 메시지도 보여줌
            showCompletionMessageSerial();
        }
    }
}

void loop()
{
    if (Serial)
    {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input == "")
        {
            // 엔터만 쳤을 때 특별한 동작 없음
        }
        else if (input == "reset")
        {
            printEmptyLines(LINE_SPACING);
            resetEEPROM();
            updateStatusLCD(); // 리셋되었으므로 O/X 화면으로 돌아옴
            Serial.println("EEPROM RESET SUCCESS!!");
            playCorrectSound();
            delay(1000);
            printEmptyLines(VERY_LONG_SPACING);
        }
        else if (input == START_STR)
        {
            printEmptyLines(LINE_SPACING);
            delay(1000);
            printProgmemString(intro);
            printEmptyLines(LINE_SPACING);
        }
        else if (input == "help")
        {
            printEmptyLines(LINE_SPACING);
            printProgmemString(gameRule);
            printEmptyLines(LINE_SPACING);
        }
        else if (input == "clear")
        {
            printEmptyLines(VERY_LONG_SPACING);
        }
        else if (input == "status")
        {
            printEmptyLines(LINE_SPACING);
            Serial.println("문제를 풀면 O, 풀지 않았다면 X로 표시됩니다.");
            Serial.println("(LCD 화면에서도 확인할 수 있습니다.)\n");
            Serial.println("  +-----+-----+-----+-----+");
            Serial.println("  |  1  |  2  |  3  |  4  |");
            Serial.println("  +-----+-----+-----+-----+");
            Serial.print("  |");
            for (int i = 0; i < NUM_PROBLEMS; i++)
            {
                if (statusEEPROM(i))
                    Serial.print("  O  |");
                else
                    Serial.print("  X  |");
            }
            Serial.println("\n  +-----+-----+-----+-----+");

            if (allProblemsCompleted())
            {
                printEmptyLines(LINE_SPACING);
                showCompletionMessageSerial();
            }
            else
            {
                printEmptyLines(LONG_SPACING);
            }
        }
        else if (input.length() == 1 && input[0] >= '1' && input[0] <= '4')
        {
            int problemNumber = input.toInt();
            handleProblem(problemNumber);
        }
        else
        {
            printEmptyLines(LINE_SPACING);
            Serial.println("input: " + input);
            Serial.println("값이 올바르지 않습니다. 다른 값을 넣어주세요.");
            printEmptyLines(VERY_LONG_SPACING);
        }
    }
}