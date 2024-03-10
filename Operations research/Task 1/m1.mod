var Mix1;
var Mix2;
maximize result: 480 * Mix1 + 350 * Mix2;	
subject to max_1: 0.6 * Mix1 + 0.4 * Mix2 <= 9.6;
subject to max_2: 0.4 * Mix1 + 0.6 * Mix2 <= 8.4;
subject to limit_1: 0 <= Mix1 <= 14;
subject to limit_2: 0 <= Mix2 <= 10;
