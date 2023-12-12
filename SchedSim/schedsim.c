// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"

typedef struct Process { 
  int pid; // Process ID 
  int bt; // Burst Time 
  int art; // Arrival Time 
  int wt; // waiting time
  int tat; // turnaround time
  int pri; // priority
}ProcessType; 

// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum){   
  int rem_bt[n];
  int t = 0; // current time

  // Store the remaining burst time of processes
  int i;
  for (i = 0; i < n; i++) {
    rem_bt[i] = plist[i].bt;
  }

  int wt[n];
  for (i = 0; i < n; i++){
    wt[i] = 0;
  }


  while (1) {
    int done = 1;
    // traverse all processes one by one repeatedly
    int i;
    for (i = 0; i < n; i++) {
      // if burst time of a process is greater than 0 then only need to process further
      if (rem_bt[i] > 0) {
        done = 0; // 

        if (rem_bt[i] > quantum) {
          // increase the value of t i.e. shows how much time a process has been processed
          t += quantum;
          // decrease the burst_time of current process by quantum
          rem_bt[i] -= quantum;
          wt[i] += t;
        } else {
          // if burst time is smaller than or equal to quantum. Last cycle for this process
          t += rem_bt[i];
          // waiting time is current time minus time used by this process
          wt[i] += t - plist[i].bt;
          // as the process gets fully executed make its remaining burst time = 0
          rem_bt[i] = 0;
        }
      }
    }

    // if all processes are done
    if (done == 1) {
      break;
    }
  }
}
 

void findTurnAroundTimeRR(ProcessType plist[], int n){
  int i;
  for (i = 0; i < n; i++){
    plist[i].tat = plist[i].bt + plist[i].wt;
  }
}

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n){ 
  /*
    * 1 Traverse until all process gets completely executed.
      - Find process with minimum remaining time at every single time lap.
      - Reduce its time by 1.
      - Check if its remaining time becomes 0 
      - Increment the counter of process completion.
      - Completion time of *current process = current_time +1;*
      - Calculate waiting time for each completed process. *wt[i]= Completion time - arrival_time-burst_time*
      - Increment time lap by one.
  */

 int rt[n]; // Remaining times

  for (int i = 0; i < n; i++) {
    rt[i] = plist[i].bt; // Initialize remaining times
  }

  int complete = 0, t = 0, min = INT_MAX;
  int shortest = 0, finish_time;
  int check = 0;

  // Process until all processes gets fully executed
  while (complete != n) {
    // Find process with minimum remaining time every time
    for (int j = 0; j < n; j++) {
      if ((plist[j].art <= t) && (rt[j] < min) && (rt[j] > 0)) {
        min = rt[j];
        shortest = j;
        check = 1;
      }
    }

    if (check == 0) {
      t++;
      continue;
    }

    // Reduce remaining time by one
    rt[shortest]--;

    // Update minimum
    min = rt[shortest];
    if (min == 0) {
      min = INT_MAX;
    }

    // If a process gets completely executed
    if (rt[shortest] == 0) {
      complete++;
      check = 0;

      // Finish time of current process
      finish_time = t + 1;

      // Calculate waiting time
      plist[shortest].wt = finish_time - plist[shortest].bt - plist[shortest].art;

      if (plist[shortest].wt < 0) {
        plist[shortest].wt = 0;
      }
    }
    // Increment time
    t++;
  }  
} 

void findWaitingTimePriority(ProcessType plist[], int n){
  qsort(plist, n, sizeof(ProcessType), my_comparer);

  findWaitingTime(plist, n);
}

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
       
  /*  
    * 1. Cast this and that into (ProcessType *)
    * 2. return 1 if this->pri < that->pri
    */ 

  //cast this and that into (ProcessType *)
  ProcessType *processThis = (ProcessType *)this;
  ProcessType *processThat = (ProcessType *)that;

  //compare based on priority
  // if (processThis -> pri < processThat -> pri){
  //   return -1;
  // }
  // else if (processThis -> pri > processThat -> pri){
  //   return 1;
  // }
  // else{
  //   return 0;
  // }

  return processThis->pri - processThat->pri;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
  //Function to find waiting time of all processes 
  findWaitingTime(plist, n); 

  //Function to find turn around time for all processes 
  findTurnAroundTime(plist, n); 

  //Display processes along with all details 
  printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
  //Function to find waiting time of all processes 
  findWaitingTimeSJF(plist, n); 

  //Function to find turn around time for all processes 
  findTurnAroundTime(plist, n); 

  //Display processes along with all details 
  printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
  //Function to find waiting time of all processes 
  findWaitingTimeRR(plist, n, quantum); 

  //Function to find turn around time for all processes 
  findTurnAroundTime(plist, n); 

  //Display processes along with all details 
  printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{
  //1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
  qsort(plist, n, sizeof(ProcessType), my_comparer);

  //2- Now simply apply FCFS algorithm.
  findWaitingTime(plist, n);
  findTurnAroundTime(plist, n);

  //Display processes along with all details 
  printf("\n*********\nPriority\n");
  printMetrics(plist, n);
}

void printMetrics(ProcessType plist[], int n)
{
  int total_wt = 0, total_tat = 0; 
  float awt, att;
  
  printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 

  // Calculate total waiting time and total turn  
  // around time 
  for (int  i=0; i<n; i++) 
  { 
      total_wt = total_wt + plist[i].wt; 
      total_tat = total_tat + plist[i].tat; 
      printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
  } 

  awt = ((float)total_wt / (float)n);
  att = ((float)total_tat / (float)n);
  
  printf("\nAverage waiting time = %.2f", awt); 
  printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  FILE *input_file = fopen(filename, "r");
  if (!input_file) {
    fprintf(stderr, "Error: Invalid filepath\n");
    fflush(stdout);
    exit(0);
  }

  // ProcessType *plist = parse_file(input_file, n);

  // Count the number of lines (processes) in the file
  int line_count = 0;
  char ch;
  while (!feof(input_file)){
    ch = fgetc(input_file);
    if (ch == '\n'){
      line_count++;
    }
  }

  fseek(input_file, 0, SEEK_SET); // Reset file pointer to the beginning

  // Allocate memory for the processes
  ProcessType *plist = (ProcessType *)malloc(line_count * sizeof(ProcessType));
  if (!plist){
    fprintf(stderr, "Error: Memory allocation failed\n");
    fclose(input_file);
    exit(0);
  }

  // Read data from the file and populate the ProcessType array
  for (int i = 0; i < line_count; i++){
    int pid, bt, art, pri;
    fscanf(input_file, "%d %d %d %d", &pid, &bt, &art, &pri);
    plist[i].pid = pid;
    plist[i].bt = bt;
    plist[i].art = 0; //all processes arrive at time 0
    plist[i].pri = pri;
    plist[i].wt = 0; // Initialize waiting time
    plist[i].tat = 0; // Initialize turnaround time
  }

  fclose(input_file);
  //update the number of processes
  *n = line_count;
  return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 