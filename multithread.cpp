#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <mutex>

using namespace std;

mutex gMutex;

typedef struct {
    int id;
    stringstream s;
}perthread;
void convert(vector<vector<int> > d,       
             int start,         
             int end,  
             stringstream &ss)
{
    for (int k = start; k < end; k++) {
         //cout<<k<<endl;
        ss<< "\t"
          << "{" << endl;
        for (int j = 0; j < 20; j++) {
            ss<< "\t\t"
              << "\""   
              << "col_" << j + 1 << "\""
              << ":" << d[k][j];
            j == 19 ? ss<< endl : ss << "," << endl;
        }
        ss<< "\t"
          << "}" << endl;
    } 
}

int main(int argc, char *argv[])
{
    double start, end;
    double start_1, end_1;

    struct timeval wall_s, wall_e;
    struct timeval wall_s1, wall_e1;


    ifstream inFile("input.csv", ios::in);
    if (!inFile) {
        cout << "開啟檔案失敗！" << endl;
        exit(1);
    }
    start = clock();  
    gettimeofday(&wall_s, NULL);
    //FILE *output=fopen("output.json","w");
    ofstream output;
    output.open("output.json");
    int num, i = 0;
    int n =atoi(argv[1]);
    int size =atoi(argv[2]);
    int count = 0;
    vector<vector<int> > data;

    string line;
    int order = 0;
    while (getline(inFile, line)) {
        istringstream sin(line);  //切割字串
        vector<int> buff;
        string pred;
        while (getline(sin, pred, '|')) {
            buff.push_back(atoi(pred.c_str()));
        }
        data.push_back(buff);
        buff.clear();
    }
    inFile.close();

    cout << "how many threads? : ";
    cin >> num;
    vector<thread> multi;
    int del = 0;
    int num_perthread = (data.size() / num) + (data.size() % num != 0);
    if (data.size() % num != 0) {
        del = (num_perthread * num) - (data.size());
    }
    perthread per[num];
    start_1 = clock();
    gettimeofday(&wall_s1, NULL);
    vector<string> out;
    for (i = 0; i < num; i++) {
        per[i].id=i;
        int start = (i * num_perthread);
        int end = (i * num_perthread) + num_perthread;
        if (i == num - 1) {
        end -= del;
    }
        multi.push_back(
            thread(convert, data, start, end, ref(per[i].s))
        );
    }
    for (int i = 0; i < multi.size(); i++) {
        multi[i].join();
    }
    end_1 = clock();
    gettimeofday(&wall_e1, NULL);
    cout << "thread wall time : " <<(long)(wall_e1.tv_sec - wall_s1.tv_sec) << endl;

    cout << "thread CPU time : " << (end_1 - start_1) / CLOCKS_PER_SEC << endl;
    output<<"{\n";
    for(int i=0;i<num;i++){
        output<<per[i].s.str();
        per[i].s.str("");
    }
    output << "}";
    output.close();
    data.clear();
    gettimeofday(&wall_e, NULL);
    end = clock();
    cout << "total time (walltime) : " << wall_e.tv_sec - wall_s.tv_sec << endl;
    cout << "total time (CPU) : " << (end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}
