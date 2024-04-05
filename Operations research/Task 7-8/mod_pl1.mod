set M;
set N;
param a{M, N};
var v;
var p1{M};

maximize result: v;

subject to s1{j in N}: sum{i in M} (a[i,j] * p1[i]) - v >= 0;
subject to s2: sum{i in M} p1[i] == 1;
subject to s3{i in M}: p1[i] >= 0;