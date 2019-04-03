#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <time.h>
#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  time_t start,end;
  start = time(NULL);
  float function = 0.0, solution = 0.0;
  for(int i=0; i<atoi(argv[4]); i++){
    function = (atof(argv[2])+((float)i+0.5)*((atof(argv[3])-atof(argv[2]))/ atof(argv[4])));
    if(atoi(argv[1])==1)
       solution = solution + f1(function,atoi(argv[5]));
    else if(atoi(argv[1])==2)
       solution = solution + f2(function,atoi(argv[5]));
    else if(atoi(argv[1])==3)
       solution = solution + f3(function,atoi(argv[5]));
    else if(atoi(argv[1])==4)
       solution = solution + f4(function,atoi(argv[5]));
  }
  float temp = (atof(argv[3])-atof(argv[2]))/ atof(argv[4]);
  solution = solution * temp;
  cout<<solution;
  int a;
  end = time(NULL);
  double execution_time = difftime(end,start);
  std::cerr<<execution_time;
  return 0;
}
