set Workers;
set Tasks;

param minWorkers;
param maxTasks;

var Assign{w in Workers, t in Tasks} binary;

/*Не входит в модель*/
/**/
maximize result: sum{w in Workers, t in Tasks} Assign[w,t];
/**/

subject to minimalWorkers{t in Tasks}: sum{w in Workers} Assign[w,t] >= minWorkers;
subject to maximumTasks{w in Workers}: sum{t in Tasks} Assign[w,t] <= maxTasks;

