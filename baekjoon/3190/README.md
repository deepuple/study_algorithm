뱀 게임

1. 사과를 지나가면, 길이가 늘어난다.
2. 꼬리나 벽에 닿으면, 게임이 끝난다.

----

게임이 끝나는 기준은 다음과 같다.
1. 시작점이 벽에 닿는가?
2. 시작점이 몸통이나 꼬리에 닿는가?(지나온 좌표를 방문하는가?)
 - 꼬리에 닿는지 확인 하기 위해 뱀의 모양을 역추적한다.
 
현재 시간, 머리 좌표, 길이, 방향, 지나온 좌표는 기록해 둘 것
지나온 좌표는 항상 길이 만큼 가지고 있다.

1. 사과를 먹을 경우, 지나온 좌표를 지우지 않는다. -> 큐를 쓴다.
2. 안먹은 경우, 가장 먼저 입력한 좌표를 지운다. 