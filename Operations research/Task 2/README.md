To run models AMPL IDE is required

The plant can produce 10 types of pumps: V1, V2, SV1, SV2, SV3, V1, V15, V2, SV2, SV3. Equipment setup time (in thousands of minutes) is given in table.

![](images/Aspose.Words.82edef5b-ac3e-4181-835f-6647a2317688.001.png)

The time spent on the production of one pump in hours is given in table.

![](images/Aspose.Words.82edef5b-ac3e-4181-835f-6647a2317688.002.png)

The pumps have the property of being replaceable.

![](images/Aspose.Words.82edef5b-ac3e-4181-835f-6647a2317688.003.png)

Each column j in row i indicates the number of pumps of type i that can replace one pump of type j.
the symbol "−" means that replacement is not possible.

For example, the number 2 in bold in the table. 3 means that instead of one V2 pump, 2 V1 pumps can be produced.

The decision on which types of pumps will be replaced is made once before production starts. Each type of pump can be replaced in the appropriate proportion shown in the table by only one.

Setting up equipment for different types of pumps begins simultaneously; the start of production of different types of pumps can be carried out in parallel.

It is required to find a time-optimal pump production plan to fulfill the order specified in table.

![](images/Aspose.Words.82edef5b-ac3e-4181-835f-6647a2317688.004.png)

Build a mathematical model and solve the problem.