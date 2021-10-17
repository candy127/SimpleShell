# SimpleShell

## 사용법
***
### ls
- ls하면 간단한 파일 목록을 보여줍니다.   
![image](https://user-images.githubusercontent.com/67225821/137606319-0840fcf9-777f-45a1-844b-cf2d9f444901.png)
- ls -l 하면 더욱 자세한 정보를 보여줍니다.   
![image](https://user-images.githubusercontent.com/67225821/137606355-cc26ca52-0692-454c-b596-90352f931f5e.png)
- ls -la 하면 숨김파일까지 보여줍니다.
![image](https://user-images.githubusercontent.com/67225821/137606421-140e83d4-5209-4825-82ca-23ddd6fb6bd3.png)
***
### mv
- mv src_path dest_path로 이동시킬 수 있습니다.
- ex) mv test test1
  - 현재 상태    
  ![image](https://user-images.githubusercontent.com/67225821/137606512-77ddd478-747e-4b44-b942-3e405ca66464.png)
  - 명령어 실행 후    
  ![image](https://user-images.githubusercontent.com/67225821/137606570-0b34c373-858c-487b-9fc0-1eb589c4e32b.png)
***
### sigint 처리   
![image](https://user-images.githubusercontent.com/67225821/137606650-499e1a2e-604b-4c15-b16d-142f74a3f97d.png)   
handler로 받아서 아무 처리도 안하고 넘김으로써 계속 이어나갈 수 있도록 했습니다.
