// To run:
// g++-9 multithreading.cpp -o prog
// ./prog 80 20 400 5 8
#include<iostream>
#include<string>
#include<fstream>
#include<thread>
#include<vector>
#include<mutex>
#include<sys/wait.h>
#include<unistd.h>
using namespace std;

mutex mtx;
int global_sum = 0;

int createfiles(int nfiles, int nnums, string prefix)
{
	int sum = 0;
	for(int i=0; i<nfiles; i++)
	{
		fstream fs;
		fs.open(
			prefix + to_string(i) + ".txt",
			fstream::out
		);
		for(int j=0; j<nnums; j++)
		{
			int rnum = rand()%10;
			fs << rnum << '\n';
			sum += rnum;
		}
		fs.close();
	}
	return sum;
}

void num_sum(int startfile_pos, int startfile, int endfile_pos, int endfile, string prefix, int rand_num )
{
	int sum = 0;
	for(int i = startfile; i<=endfile ; i++)
	{
		fstream fs;
		fs.open(
			prefix + to_string(i) + ".txt",
			fstream::in
		);
		int line_pos = 0;
		if(i==startfile) {
			string ignore;
			while(line_pos!=startfile_pos) {
				getline(fs,ignore);
				line_pos++;				
			}
		}
		int line_end = -1;
		if(i==endfile) line_end = endfile_pos + 1;
		else line_end = rand_num;

		while(line_pos!=line_end) {
			string num;
			getline(fs,num);
			sum += stoi(num);
			line_pos++;
		}
	}
	mtx.lock();
	//cout<<"pre "<<prefix<<" sp"<<startfile_pos<<" sf"<<startfile<<" efp"<<endfile_pos<<" ef"<<endfile<<" localsum :"<<sum<<endl;
	global_sum += sum;
	mtx.unlock();
}


int main(int argc, char* argv[])
{
	if(argc!=6)
	{
		cout<<"not valid"<<endl;
		return 1;
	}

	int a_files=atoi(argv[1]);
	int b_files=atoi(argv[2]);
	int rand_num=atoi(argv[3]);
	int a_threads=atoi(argv[4]);
	int b_threads=atoi(argv[5]);

	int asum = createfiles(a_files, rand_num, "a_");
	int bsum = createfiles(b_files, rand_num, "b_");
	int actualsum = asum + bsum;

	int pfds[2];
	pipe(pfds);

	pid_t pid = fork();

	if(pid!=0)
	{
		//parent process
		cout << "actual sum" << actualsum << endl;

		const int total_aelems = a_files * rand_num;
		const int eqwork_a = (total_aelems/a_threads);
 		const int residuework_a = (total_aelems%a_threads);

		vector <thread> parent_threads;
		int work_done = 0;
		int startfile_pos = -1;
		int startfile = -1;
		int endfile_pos = -1;
		int endfile = -1;
		string prefix = "a_";
		for(int i=0;i<a_threads;i++)
		{
			startfile_pos = work_done % rand_num;
			startfile = work_done / rand_num;
			if(i == 0) work_done += (eqwork_a + residuework_a);
			else work_done += eqwork_a;
			endfile_pos = (work_done - 1) % rand_num;
			endfile = (work_done - 1) / rand_num;

			parent_threads.push_back(thread(
				num_sum,
				startfile_pos,
				startfile,
				endfile_pos,
				endfile,
				prefix,
				rand_num
			));
		}
		for(int i=0;i<a_threads;i++)
		{
			parent_threads[i].join();
		}
		wait(NULL);
		close(pfds[1]);
		int child_total_sum = 0;
		read(pfds[0], &child_total_sum, sizeof(int));
		close(pfds[0]);
		cout << "parent sum: " << global_sum << endl;
		cout<< "child sum: " << child_total_sum << endl;
		cout << "total sum: " << global_sum + child_total_sum << endl; 

	}
	else
	{
		//child process

		const int total_belems = b_files * rand_num;
		const int eqwork_b = (total_belems/b_threads);
 		const int residuework_b = (total_belems%b_threads);

		vector <thread> child_threads;
		int work_done = 0;
		int startfile_pos = -1;
		int startfile = -1;
		int endfile_pos = -1;
		int endfile = -1;
		string prefix = "b_";

		for(int i=0;i<b_threads;i++)
		{

			startfile_pos = work_done % rand_num;
			startfile = work_done / rand_num;
			if(i == 0) work_done += (eqwork_b + residuework_b);
			else work_done += eqwork_b;
			endfile_pos = (work_done - 1) % rand_num;
			endfile = (work_done - 1) / rand_num;

			
			child_threads.push_back(thread(
				num_sum,
				startfile_pos,
				startfile,
				endfile_pos,
				endfile,
				prefix,
				rand_num
			));
		}
		for(int i=0;i<b_threads;i++) {
			child_threads[i].join();
		}
		close(pfds[0]);
		write(pfds[1], &global_sum, sizeof(int));
		close(pfds[1]);
	}
	return 0;
}
