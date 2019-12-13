# Process-scheduler
Process scheduler takes input from text file containing the name of the
process, arrival time, burst time, and priority. Then it calculates the average waiting time of
the processes and gives us the output. Inside this program there are four functions and they
are:
• First Come First Serve (FCFS): In this function, the processes are
scheduled according to their arrival time and whoever comes first gets a
slot first. When more than one process arrives at the same time then the
first one in pipeline is chosen.
• Shortest Job First (SJF): In this function, the process with the shortest
burst time is chosen first and gets a slot.
• Round Robin (RR): Here, a fixed time is allotted to each process, called
quantum, for execution. Once a process is executed for given time
period that process is preempted and other process executes for given
time period. Context switching is used to save states of preempted
processes.
• Priority Scheduling (PR): In this function, the one with the highest
priority is executed at first and then processes with lower priority are
executed. If two or more process has similar priority then SJF, FCFS and
RR is used correspondingly. 

# Sample Input: (Put process name, AT, BT, Priority, time quantum accordingly)
ProcessName ArrivalTime BurstTime Priority
P0          1           7          3
P1          2           4          1
P2          3           10         2
TimeQuantum: 3

# Sample Output:
First Come First Serve: 5.00 ms
Shortest Job First: 4.33 ms
Priority Scheduling: 5.67 ms
Round Robin: 9.00 ms

Best scheduling algorithm for the given processes: Shortest Job First
