set I; # Индексы для станков и типов насосов
param p{I} >= 0; # Количество насосов каждого типа, которые нужно произвести
param r{I} > 0; # Скорость производства каждого станка
param t{I} >= 0; # Время настройки каждого станка
param n{I,I} >= 0; # Количество насосов типа i, которые заменяют насос типа j
param y{I} binary; # Показывает, необходим ли насос
var x{I} >= 0; # Количество времени, в течение которого станок i работает
var z{I,I} binary; # Равна 1, если насосы типа i заменяются насосами типа j

minimize result: sum{i in I} (x[i] + t[i] * sum{j in I} z[i,j]);

subject to production{i in I}: x[i] >= sum{j in I} n[i,j]*z[i,j]*p[j]*r[i];
subject to only_one_pump{j in I}: sum{i in I} z[i,j] <= 1;
subject to produce_pumps_when_we_need_it{j in I}: sum{i in I} z[i,j] >= y[j];
subject to replace_only_when_we_can{i in I, j in I}: z[i,j] <= n[i,j];

