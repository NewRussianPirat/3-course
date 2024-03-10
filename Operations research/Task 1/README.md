To run models AMPL IDE is required

Task 1

Task formulation:

The workshop produces two types of mixtures of cement and sand. The percentage of cement and sand in the mixtures, profit from the sale of 1 ton of the mixture, reserves of cement and sand and the maximum consumption of each mixture are given in the table. Construct an economic and mathematical model of the problem and draw up an optimal plan for the production of mixtures.

<table><tr><th rowspan="2" vertical-align="top">Components</th><th colspan="2" vertical-align="top">Mixtures</th><th vertical-align="top">Reserves</th></tr>
<tr><td vertical-align="top">№1</td><td vertical-align="top">№2</td><td vertical-align="top"></td></tr>
<tr><td vertical-align="top">Cement</td><td vertical-align="top">60%</td><td vertical-align="top">40%</td><td vertical-align="top">9.6 t</td></tr>
<tr><td vertical-align="top">Sand</td><td vertical-align="top">40%</td><td vertical-align="top">60%</td><td vertical-align="top">8.4 t</td></tr>
<tr><td vertical-align="top">Profit</td><td vertical-align="top">480</td><td vertical-align="top">350</td></tr>
<tr><td vertical-align="top">Maximum consumption</td><td>14 t</td><td>10 t</td></tr>
</table>

Task 2

Solve an integer programming problem

c'x→max, Ax ≤ b, x<sub>j</sub> ≥ 0, x<sub>j</sub> – integer.

Matrix A:

<table>
<tr><td>1</td><td>-5</td><td>3</td><td>1</td><td>0</td><td>0</td></tr>
<tr><td>4</td><td>-1</td><td>1</td><td>0</td><td>1</td><td>0</td></tr>
<tr><td>2</td><td>4</td><td>2</td><td>0</td><td>0</td><td>1</td></tr>
</table>

Vector b:

<table>
<tr><td>-8</td><td>22</td><td>30</td></tr>
</table>

Vector с:

<table>
<tr><td>7</td><td>-2</td><td>6</td><td>0</td><td>5</td><td>2</td></tr>
</table>

Task 3

Solve an integer programming problem

c'x→max, Ax ≤ b, d<sub>j<sub>1</sub></sub> ≤ x<sub>j</sub> ≤ d<sub>j<sub>2</sub></sub>, x<sub>j</sub> – integer.

Matrix А:

<table>
<tr><td>1</td><td>0</td><td>0</td><td>12</td><td>1</td><td>-3</td><td>4</td><td>-1</td></tr>
<tr><td>0</td><td>1</td><td>0</td><td>11</td><td>12</td><td>3</td><td>5</td><td>3</td></tr>
<tr><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>22</td><td>-2</td><td>1</td></tr>
</table>

Vector b:

<table>
<tr><td>40</td><td>107</td><td>61</td></tr>
</table>

Vector c:

<table>
<tr><td>2</td><td>1</td><td>-2</td><td>-1</td><td>4</td><td>-5</td><td>5</td><td>5</td></tr>
</table>

Vector d<sub>1</sub>:

<table>
<tr><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

Vector d<sub>2</sub>:

<table>
<tr><td>3</td><td>5</td><td>5</td><td>3</td><td>4</td><td>5</td><td>6</td><td>3</td></tr>
</table>
