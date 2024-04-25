var Table integer;
var Closet integer;
maximize result: 10 * Table + 8 * Closet;
subject to s1: 0.2 * Table + 0.1 * Closet <= 40;
subject to s2: 0.1 * Table + 0.3 * Closet <= 45;
subject to s3: 1.2 * Table + 1.5 * Closet <= 360;