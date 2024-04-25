set Workers;
set Tasks;

param minWorkers;

var Assign{w in Workers, t in Tasks} binary;

/*Не входит в модель*/
/**/
minimize result: sum{w in Workers, t in Tasks} Assign[w,t];
/**/

subject to minimalWorkers{t in Tasks}: sum{w in Workers} Assign[w,t] >= minWorkers;

