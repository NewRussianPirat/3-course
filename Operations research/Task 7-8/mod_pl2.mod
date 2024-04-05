set M;
set N;
param a{M, N};
var v;
var p2{N};

minimize result: v;

subject to s1{i in M}: sum{j in N} (a[i,j] * p2[j]) - v <= 0;
subject to s2: sum{j in N} p2[j] == 1;
subject to s3{j in N}: p2[j] >= 0;