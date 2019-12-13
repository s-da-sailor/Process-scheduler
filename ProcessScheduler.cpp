#include <bits/stdc++.h>
using namespace std;

#define SIZE 100005                 // Maximum size of process list, also max arrival time we can work with
#define LL long long

struct Process {                    // Structure for processes
    string name;
    LL id, AT, BT, PR;              // pid, Arrival time, Burst time, Priority
    Process() { }                   // default constructor
    Process(string pn, LL i, LL a, LL b, LL p) {
        name = pn;                  // custom constructor
        id = i;
        AT = a;
        BT = b;
        PR = p;
    }
};

bool comp(const Process &a, const Process &b) { // comparator for sorting the processes
    if(a.AT==b.AT) return a.id<b.id;            // by arrival time
    return a.AT<b.AT;
}

struct compareSJF {
    bool operator() (const Process &a, const Process &b) const {
        if(a.BT==b.BT && a.AT==b.AT) return a.id>b.id;  // comparator for priority queue in SJF
        else if(a.BT==b.BT) return a.AT>b.AT;
        else return a.BT>b.BT;
    }
};

struct comparePRS {
    bool operator() (const Process &a, const Process &b) const {
        if(a.PR==b.PR && a.AT==b.AT) return a.id>b.id;  // comparator for priority queue in PRS and RR
        else if(a.PR==b.PR) return a.AT>b.AT;
        else return a.PR>b.PR;
    }
};

struct FCFS {
    LL N;                       // Number of processes
    vector<Process> PList;      // Process list

    double fcfs() {
        sort(PList.begin(), PList.end(), comp);         // sorting according to arrival time

        LL currT = 0, sum = 0;

        for(int i=0; i<PList.size(); i++) {
            LL id = PList[i].id, AT = PList[i].AT, BT = PList[i].BT;

            if(AT>currT) currT = AT;                    // for handling the cases where AT>currT

            LL temp = currT - AT;                       // waiting time

            sum += temp;                                // update sum

            currT += BT;                                // update current time
        }

        double ret = (double)sum/(double)N;             // get average waiting time
        return ret;
    }
} FCFS;

struct SJF {
    LL N;                       // Number of processes
    LL prev[SIZE];              // Array for handling previous release time while calculating waiting time
    vector<Process> PList;      // Process list
    priority_queue<Process, vector<Process>, compareSJF> PQ;    // priority queue for current available processes

    double sjf() {              // calculates average waiting time for SJF
        sort(PList.begin(), PList.end(), comp);     // sorting list of process according to arrival time
        memset(prev, -1, sizeof(prev));             // initializing prev

        LL sum = 0;                                 // current sum
        LL currT = 0, currP = 0;                    // current time and current position in vector
        LL previd = -1;                             // There is no previously used id

        while(1) {
            if(PQ.empty() && currP>=N) break;       // we are done

            if(PQ.empty()) {
                PQ.push(PList[currP]);              // push in the current available processes
                currP++;                            // update current position
            }

            else if(!PQ.empty()) {
                Process top = PQ.top();             // we will work with this process now
                PQ.pop();                           // discard temporarily

                string name = top.name;
                LL id, AT, BT, PR;
                id = top.id; AT = top.AT; BT = top.BT; PR = top.PR;

                if(AT>currT) currT = AT;            // handle the cases when AT>currT

                if(id!=previd) {                    // change in previous id (new calculation)
                    if(prev[id]==-1) {              // This process is the first to calculate
                        sum += (currT-AT);          // Get waiting time and update sum

                        prev[id] = currT;           // update prev

                        if(previd!=-1) prev[previd] = currT;
                    }
                    else {
                        sum += (currT-prev[id]);    // get waiting time and update sum

                        prev[id] = currT;           // update prev

                        if(previd!=-1) prev[previd] = currT;
                    }
                    previd = id;
                }
                else {
                    prev[id] = currT;
                }

                BT--;                               // Burst time is decreased

                currT++;                            // Current time is increased

                while(currP<N && PList[currP].AT<=currT) {  // get the processes which have
                    string currName = PList[currP].name;    // arrival time <= current time
                    LL currId = PList[currP].id;            // and push them in available processes
                    LL currAT = PList[currP].AT;
                    LL currBT = PList[currP].BT;
                    LL currPR = PList[currP].PR;
                    PQ.push(Process(currName, currId, currAT, currBT, currPR));
                    currP++;
                }

                if(BT>0) PQ.push(Process(name, id, AT, BT, PR));    // push the current process in available processes
            }
        }

        double ret = (double)sum/(double)N;         // Get average waiting time
        return ret;
    }
} SJF;

// Here everything is same as SJF apart from the ordering criteria of the priority queue
struct PRS {
    LL N;                   // Number of processes
    LL prev[SIZE];          // Prev array (similar as SJF)
    vector<Process> PList;  // Process list
    priority_queue<Process, vector<Process>, comparePRS> PQ;    // priority queue for current available processes

    double prs() {          // calculates average waiting time for PRS
        sort(PList.begin(), PList.end(), comp); // sorting list of process according to arrival time
        memset(prev, -1, sizeof(prev));         // initializing prev

        LL sum = 0;                             // current sum
        LL currT = 0, currP = 0;                // current time and current position in vector
        LL previd = -1;                         // There is no previously used id

        while(1) {
            if(PQ.empty() && currP>=N) break;   // we are done

            if(PQ.empty()) {
                PQ.push(PList[currP]);          // push in the current available processes
                currP++;                        // update current position
            }

            else if(!PQ.empty()) {
                Process top = PQ.top();         // we will work with this process now
                PQ.pop();                       // discard temporarily

                string name = top.name;
                LL id, AT, BT, PR;
                id = top.id; AT = top.AT; BT = top.BT; PR = top.PR;

                if(AT>currT) currT = AT;        // handle the cases when AT>currT

                if(id!=previd) {                // change in previous id (new calculation)
                    if(prev[id]==-1) {          // This process is the first to calculate
                        sum += (currT-AT);      // Get waiting time and update sum

                        prev[id] = currT;       // update prev

                        if(previd!=-1) prev[previd] = currT;
                    }
                    else {
                        sum += (currT-prev[id]);    // get waiting time and update sum

                        prev[id] = currT;           // update prev

                        if(previd!=-1) prev[previd] = currT;
                    }
                    previd = id;
                }
                else {
                    prev[id] = currT;
                }

                BT--;                   // Burst time is decreased

                currT++;                // Current time is increased

                while(currP<N && PList[currP].AT<=currT) {  // get the processes which have
                    string currName = PList[currP].name;    // arrival time <= current time
                    LL currId = PList[currP].id;            // and push them in available processes
                    LL currAT = PList[currP].AT;
                    LL currBT = PList[currP].BT;
                    LL currPR = PList[currP].PR;
                    PQ.push(Process(currName, currId, currAT, currBT, currPR));
                    currP++;
                }

                if(BT>0) PQ.push(Process(name, id, AT, BT, PR)); // push the current process in available processes
            }
        }

        double ret = (double)sum/(double)N;                      // Get average waiting time
        return ret;
    }
} PRS;

// Here is everything is almost same as PR. But we take advantage of the priority queue
// We get the top element(most important) and after working with it we make it least important
// So the order we get the top elements from the priority queue is like Round Robin
struct RR {
    LL N;                   // Number of processes
    LL Q;                   // Time Quantum
    LL prev[SIZE];          // Prev array
    vector<Process> PList;  // Process list
    priority_queue<Process, vector<Process>, comparePRS> PQ; // priority queue for current available processes

    double rr() {
        sort(PList.begin(), PList.end(), comp); // sorting list of process according to arrival time
        memset(prev, -1, sizeof(prev));         // initializing prev

        LL sum = 0;                             // current sum
        LL currT = 0, currP = 0;                // current time and current position in vector
        LL previd = -1;                         // There is no previously used id
        LL currPR = 0;                          // Current priority (we will gradually increase this)

        while(1) {
            if(PQ.empty() && currP>=N) break;   // we are done

            if(PQ.empty()) {                    // push in the current available processes
                string pName = PList[currP].name;
                LL pId = PList[currP].id;
                LL pAT = PList[currP].AT;
                LL pBT = PList[currP].BT;
                currPR++;                       // Increase current priority
                PQ.push(Process(pName, pId, pAT, pBT, currPR));
                currP++;
            }

            else if(!PQ.empty()) {              // we will work with this process now
                Process top = PQ.top();
                PQ.pop();                       // discard temporarily

                string name = top.name;
                LL id, AT, BT, PR;
                id = top.id; AT = top.AT; BT = top.BT; PR = top.PR;

                if(AT>currT) currT = AT;        // handle the cases when AT>currT

                if(id!=previd) {                // change in previous id (new calculation)
                    if(prev[id]==-1) {          // This process is the first to calculate
                        sum += (currT-AT);      // Get waiting time and update sum

                        prev[id] = currT;       // update prev

                        if(previd!=-1) prev[previd] = currT;
                    }
                    else {
                        sum += (currT-prev[id]);// get waiting time and update sum

                        prev[id] = currT;       // update prev

                        if(previd!=-1) prev[previd] = currT;
                    }
                    previd = id;
                }
                else {
                    prev[id] = currT;
                }

                LL dec = min(Q, BT);         // Time to decrease

                BT -= dec;                   // Burst time is decreased (According to time quantum)

                currT += dec;                // Current time is increased

                while(currP<N && PList[currP].AT<=currT) {  // get the processes which have
                    string currName = PList[currP].name;    // arrival time <= current time
                    LL currId = PList[currP].id;            // and push them in available processes
                    LL currAT = PList[currP].AT;
                    LL currBT = PList[currP].BT;
                    currPR++;                               // Increase current priority
                    PQ.push(Process(currName, currId, currAT, currBT, currPR));
                    currP++;
                }

                if(BT>0) {
                    currPR++;
                    PQ.push(Process(name, id, AT, BT, currPR)); // push the current process in available processes
                }
            }
        }

        double ret = (double)sum/(double)N;                 // Get average waiting time
        return ret;
    }
} RR;

vector<Process> PV;

int main() {
    //freopen("input.txt", "r", stdin);

    string line;
    int init = 1;
    int cnt = 0;
    int flag = 1;
    LL Q;

    while(getline(cin, line)) {
        if(init) {      // First line
            init = 0;
            continue;
        }

        stringstream is(line);

        string Pname;
        LL id, AT, BT, PR;

        id = cnt;
        cnt++;

        is >> Pname;
        if(Pname[0]=='T') { // Reached at the end
            is >> Q;
            flag = 0;
            break;
        }
        is >> AT >> BT >> PR;

        if(!flag) break;

        PV.push_back(Process(Pname, id, AT, BT, PR)); // Push to process list
    }

    double fcfs, sjf, prs, rr;

    FCFS.PList = PV;
    FCFS.N = PV.size();
    fcfs = FCFS.fcfs();                 // Calculate for FCFS
    cout << "First Come First Serve: " << setprecision(2) << fixed << fcfs << " ms\n";

    SJF.PList = PV;
    SJF.N = PV.size();
    sjf = SJF.sjf();                    // Calculate for SJF
    cout << "Shortest Job First: " << setprecision(2) << fixed << sjf << " ms\n";

    PRS.PList = PV;
    PRS.N = PV.size();
    prs = PRS.prs();                    // Calculate for PRS
    cout << "Priority Scheduling: " << setprecision(2) << fixed << prs << " ms\n";

    RR.PList = PV;
    RR.N = PV.size();
    RR.Q = Q;
    rr = RR.rr();                       // Calculate for RR
    cout << "Round Robin: " << setprecision(2) << fixed << rr << " ms\n";

    double mn = 1e8;
    string mnStr = "";
    if(fcfs<mn) {
        mn = fcfs;
        mnStr = "First Come First Serve";
    }
    if(sjf<mn) {                        // Determine the best
        mn = sjf;
        mnStr = "Shortest Job First";
    }
    if(prs<mn) {
        mn = prs;
        mnStr = "Priority Scheduling";
    }
    if(rr<mn) {
        mn = rr;
        mnStr = "Round Robin";
    }

    cout << "\nBest scheduling algorithm for the given processes: " << mnStr << "\n";

    return 0;
}
