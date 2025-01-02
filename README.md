# Practial Design of Application Circuits for Microprocessors
## 1. Introduction
본 프로젝트는 **X-Hyper320TKU**라는 교육용 임베디드 장비로 Keypad, Dot Matrix, CLCD, FND, LED, FND을 제어하여 '4인용 눈치 게임'을 구현한 것이다. X-Hyper320TKU은 CPU 모듈로 PXA320(806Mhz) 프로세서를 사용한다. PXA320 프로세서는 Intel이 설계한 ARMv5TE 기반 XScale 코어를 사용하는 고성능 애플리케이션 프로세서이다. 최대 806Mhz의 클럭 속도를 제공하며, 저전력 설계와 멀티미디어 가속기(MAC)를 포함해 스마트폰, PDA, 산업용 터미널 등 다양한 디바이스에서 멀티미디어 작업과 임베디드 애플리케이션에 최적화된 성능을 발휘한다. 

## 2. Game Rule
본 장비에는 4x4의 Keypad가 존재한다. Keypad의 각 열(column)은 4명의 플레이어에게 개별로 할당된다. 4명의 플레이어 중 1명이 attacker로 선정되어, 자신의 열에서 두 개의 칸을 임의로 누른다. 그리고나서 나머지 3명의 플레이어가 순서대로 자신의 열에서 두 개의 칸을 임의로 누른다. 이때 attacker가 누른 Keypad의 행(row)과 일치한다면 감점을 당한다. 또한, 플레이어가 자신의 Keypad가 아닌 다른 플레이어의 Keypad을 눌러도 감점된다. 이후 다른 사람이 attacker로 선정되며, 어떤 1명의 점수가 0점이 될 때까지 게임은 계속된다. 각각에게 부여된 기본 점수는 5점이다.

## 3. Flowchart
![image](https://github.com/user-attachments/assets/16173237-54a4-46c1-b2b8-a4cad6469d26)

![image](https://github.com/user-attachments/assets/4ca916c4-8aad-4beb-aeac-22d11357a0d7)

![image](https://github.com/user-attachments/assets/72d867c1-7581-4995-992c-4a68f85efef8)

![image](https://github.com/user-attachments/assets/5b01fb97-df3b-4da0-83f4-0fc88bf71398)

![image](https://github.com/user-attachments/assets/d5b8e16d-4ac9-4985-bd44-03bb8b84c231)

![image](https://github.com/user-attachments/assets/79482f08-5819-4698-82fb-f328e1e654f4)

![image](https://github.com/user-attachments/assets/ac677cab-3d1c-4940-9ac3-8b95bd75baf4)

![image](https://github.com/user-attachments/assets/1421a601-15c8-41cc-bdb4-d0d6050e28da)

![image](https://github.com/user-attachments/assets/540a0cef-693b-41d8-80b0-865424e411f7)

![image](https://github.com/user-attachments/assets/1fba6b50-7ddc-479f-baae-67d93f0ab0c6)

![image](https://github.com/user-attachments/assets/9873d7b8-8367-40bb-bff8-69dd493a723c)

## 4. Problem and Solution
Keypad을 눌렀다가 뗄 때, 물리적 접점이 완전히 닫히거나 열리기 전에 미세한 진동이나 튕김(bounce)이 발생한다. 이때 짧은 시간 동안 접점이 빠르게 열리고 닫히며 불안정한 신호가 생성되는데, 디지털 회로에서는 이러한 빠른 신호 변화를 여러 번의 입력으로 오인할 수 있다. 즉, 이러한 현상을 **Bounce 현상** 이라고 하며, 입력 데이터를 처리할 때 잘못된 신호를 유발하며, 가령 한 번 눌러야 할 키각 여러 번 눌린 것으로 인식될 수 있다.

이를 해결하기 위해 **Debounce**라는 방법이 제안된다. 이는 하드웨어 또는 소프트웨어에 의해 구현될 수 있다. 본 프로젝트에서는 Key 입력이 감지된 후, 지정된 시간(ms 단위)동안 추가 입력을 무시하도록 하는 코드를 채택하여, 소프트웨어적인 방법으로 잘못된 다중 입력을 방지하여 시스템 안정성을 향상시켰다.
