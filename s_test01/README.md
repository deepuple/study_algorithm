S사 시험문제

1. 패턴 n 개중 r개를 골라, 
2. r개의 패턴을 각각 90, 180, 270도로 돌려 합쳐서
3. 원하는 패턴이 만들어지는지 본다
* 패턴은 5x5의 boolean 배열로 만들어진다 
----

1. 일단 패턴 선택의 조합을 만든다. combination --> comb[](사이즈 r)
2. 조합이 만들어지면, 각 아이템에 대한 회전(0,90,180,270)을 완전 탐색(재귀)으로 구현한다 --> 각 아이템 마다 4개의 선택이 가능함. 따라서, 3개 아이템이 있고 각 4개의 선택권이 있을때, 3을 depth로 해서 중복 선택이 가능하도록 깊이 탐색으로 구현한다. --> brute[](사이즈 r)
3. [a,b,c], [0,90,180] 이면,  trans(a,0)+trans(b,90)+trans(c,180) 이 원하는 패턴인지 확인한다.
