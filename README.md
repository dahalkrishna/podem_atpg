Implementation of PODEM to generate test patterns for stuck-at faults and calculate the fault coverage on the ISCAS85 benchmark files. The program also has a serial logic simulator that reads from a pattern file and displays the values at the output port at the terminal.
Code was implemented as a part of coursework. A single pattern per fault is generated The algorithm drops trying to find a test pattern for a fault if it takes more than 200ms and prints a Timeout
The result files are in the format:   
_Example:   
1 /0	Success  
1 /1	Success  
2 /0	Success  
.  
.  
1316 /0 Timeout  
1316 /1 Failure  
.  
.  
No of of Failures = 0  
No of of Timeouts = 0  
Coverage = 98 %_


|  Circuit  | Coverage(%)  |
|:---:|:---:|
|  c17    |  100  |
|  c880   |  98  |
|  c1355  |  94  |
|  c1908  |  95  |
|  c2670  |  94  |
|  c3450  |  85  |
|  c5315  |  99  |
|  c7552  |  98  | 

Compile the code using **make** command and execute the code using **./project c17.isc c17.pattern c17.res**. 
