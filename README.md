# Arduino Hacking Badge

## Thanks to

1. badge_example 문제 개발진 - 김민수
   - (해킹 팀 'VOID' 부팀장 | 덕영고등학교 정보보호 동아리 'MAKE@Infosec' 부장)
2. badge_old 문제 개발진 - 박세진
   - (해킹 팀 'VOID' 팀원 | 덕영고등학교 정보보호 동아리 'MAKE@Infosec' 팀원)

## 개요

이 프로젝트는 실제 큰 규모의 CTF 본선 같은곳에서 배포하는 뱃지 해킹을 모방하여 만들기 위한 프로젝트입니다.

간단하게 문제를 풀이하면 문제 index에 맞는 LED가 점등 하고, 전체 풀이 시 LED 애니메이션으로 표현됩니다.

이 외에도 시리얼 모니터를 통해 Shell을 모방한 인터페이스를 제공하여, 사용자가 문제를 선택하고 풀이 상태를 확인할 수 있도록 구현하였습니다.

[badge_example.ino](/badge_example/badge_example.ino) 파일을 참고하여 아스키 아트 예시를 포함한 문제 설명을 확인할 수 있습니다.

## 하드웨어 요구사항

- 아두이노 보드 (Arduino Uno/Nano 권장)
- LED 4개 (핀 6, 7, 8, 9에 연결 권장, 다른 핀 사용 가능)
- 시리얼 통신용 USB 케이블

## 기능 소개

### 기본 게임 구조

- 4개의 독립적인 퍼즐/문제 제공
- 각 문제에 고유한 LED 피드백
- 문제 해결 상태는 EEPROM에 저장되어 전원이 꺼져도 유지
- 시리얼 모니터를 통한 Shell 모방 인터페이스

### 주요 명령어 (예제코드 기준)

- `dyhs`: 게임 시작 (메인 화면으로 이동)
- `help`: 게임 도움말 표시
- `reset`: 게임 진행 상태 초기화 (EEPROM 리셋)
- `clear`: 화면 지우기
- `1`~`4`: 해당 번호의 문제 선택
- `exit`: 현재 문제에서 나가기
- `status`: 현재 문제 풀이 상태 확인

## 기술적 특징

### 메모리 최적화

- 큰 텍스트(아스키아트, 문제 설명 등)는 PROGMEM을 통해 프로그램 메모리에 저장
- 최소한의 RAM 사용으로 메모리 효율성 극대화

### 시각적 피드백

- 문제별 LED 할당으로 직관적인 피드백 제공
- 정답/오답에 따른 다양한 LED 깜빡임 패턴
- 문제 전체 해결 시 특별 LED 애니메이션

### 상태 저장

- EEPROM을 통한 게임 진행 상태 영구 저장
- 전원이 꺼져도 이전 진행 상태 유지

## 커스터마이징 가이드

다음 부분을 수정하여 게임을 쉽게 커스터마이징할 수 있습니다:

### 시리얼 시작 문자열 변경

```cpp
#define START_STR "START_GAME"
```

게임 시작하기 위해 시리얼 시작 문자열을 변경할 수 있습니다.

### 문제 및 정답 변경

```cpp
const char *ANSWERS[NUM_PROBLEMS] = {"FLAG{1}", "FLAG{2}", "FLAG{3}", "FLAG{4}"};
```

각 인덱스에 해당하는 문제의 정답을 변경할 수 있습니다.

### 문제 내용 (아스키 아트) 변경

```cpp
const char clear[] PROGMEM;
const char intro[] PROGMEM;
const char gameRule[] PROGMEM;
const char p1[] PROGMEM;
const char p2[] PROGMEM;
const char p3[] PROGMEM;
const char p4[] PROGMEM;
const char allSolve[] PROGMEM;
```

문제 설명, 시작 화면, 게임 설명, 전체 풀이 축하 아스키코드 등을 저장합니다. 이 부분은 아스키아트를 포함한 큰 문자열이 주로 저장됩니다.

위 변수의 내용을 어떻게 넣어야 하는지 모르겠다면 [badge_example.ino](/badge_example/badge_example.ino) 파일을 참고하세요. (이 파일은 아스키아트 예시를 포함하고 있습니다.)

### LED 핀 변경

```cpp
#define LED_PIN_1 9
#define LED_PIN_2 8
#define LED_PIN_3 7
#define LED_PIN_4 6
```

문제를 추가하거나 다른 핀을 사용하려면 이 정의를 변경하세요.

### 문제 수 변경

```cpp
#define NUM_PROBLEMS 4
```

문제 수를 변경할 때는 이 상수와 함께 관련 정의(#define)와 배열(ANSWERS, LED_PINS, problemTexts 등)의 크기도 함께 조정해야 합니다.

## 시작하기

1. 아두이노 보드에 코드 업로드
2. 시리얼 모니터 열기 (4800 baud 설정)
3. `GAME_START` 입력하여 게임 시작
4. `help` 입력으로 게임 방법 확인
5. 번호(1-4)를 입력하여 문제 풀이 시작

## 기술 스택

- Arduino IDE
- C/C++
- EEPROM 라이브러리
- PROGMEM (플래시 메모리 최적화)

## 주의사항

- 아두이노 보드 메모리 한계로 인해 큰 용량의 아스키 아트는 메모리 부족 문제 발생 시킴. 알잘딱하게 조정하여 사용.
- 시리얼 통신 속도는 4800 baud로 설정되어 있으며, 이는 시리얼 감성(?)값이므로 필요 시 조정 가능.
- `reset` 명령어는 모든 게임 진행 상태를 초기화하니 주의.

## 코드 구조

- **setup()**: 초기 설정 (LED 핀, 시리얼 통신 등)
- **loop()**: 사용자 입력 처리 및 게임 흐름 제어
- **handleProblem()**: 개별 문제 풀이 로직
- **유틸리티 함수**: LED 제어, 텍스트 출력, EEPROM 관리 등

## 라이선스

이 프로젝트는 MIT 라이선스를 따릅니다. 라이선스에 대한 자세한 내용은 LICENSE 파일을 참조하세요.
