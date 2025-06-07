# 컴퓨터 그래픽스 과제 8 - Rendering Meshes
1. 결과 스크린샷  

- [Q1: Immediate Mode ]

![image](https://github.com/user-attachments/assets/038008f4-299c-468c-9b55-eeb68e7fb07a)

- [Q2: Vertex Arrays]

![image](https://github.com/user-attachments/assets/0b136824-9f88-498f-83a7-fe0859fb611c)


2. 실행 방법

- zip파일을 다운로드 하고 압축 해제합니다.  
- OpenglViewer.sln 파일을 열어 빌드하시면 됩니다.
- Q1은 Q1_Immediate_mode 프로젝트에, Q2는 Q2_Vertex_Arrays프로젝트에 있습니다.

3. Report FPS

- Q1:
  처음부터 1100 FPS정도에서 멈춰있는 모습을 보였습니다.
- Q2:
  1575.xx FPS에서 시작해 계속 증가하다가 30초정도 뒤, 1912~1913 FPS 정도에서 멈춰있는 모습을 보였습니다.
->  결론: 약 1.7배 정도 성능 향상이 나타났습니다. 이유는 Immediate Mode는 정점 하나당 수많은 API 호출이 일어나 CPU-GPU 전송 오버헤드가 크지만,  
       Vertex Arrays 방식은 데이터를 GPU에 미리 올려 호출 횟수를 최소화하여 효율적으로 렌더링하기 때문으로 보입니다.

       
과제 요구사항에 따라 모든 외부 라이브러리를 포함하여 설치가 필요하지 않도록 구성했습니다.  
지피티의 도움을 참고했습니다.  
