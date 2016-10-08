#include <cmath>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

struct city
{
  int cityNum;
  int lat;
  int lon;
};

class annealing
{
private:
  int numCities, solCount;
  double tourLength, bestLength;
  string outFileName;
  vector<int> tour;
  vector<int> bestTour;                               //the order of the route
  vector<city> cities;                            //cities and the latitude and longitude
public:
  void randTourSolution()
  {
    std::random_shuffle(tour.begin(), tour.end());
    for (int i = 0; i < numCities; i++)
    {
      bestTour[i] = tour[i];
    }
  }
  void swapNeighbors(vector<int> &tempTour)
  {
    int a = 0, b = 0;
    while(a == b)
    {
      a  = rand() % numCities;                  //randomly choosing two cities
      b = rand() % numCities;
    }
    int temp = tempTour[a];                     //swapping those two cites to change
    tempTour[a] = tempTour[b];                  //order of tour
    tempTour[b] = temp;
  }
  void calcTourLength(vector<int> &tempTour, double &length)
  {
    length = 0;
    double a, b;
    for (int i = 0; i < numCities - 1; i++)
    {
      a = (double)cities[tempTour[i + 1]].lat - (double)cities[tempTour[i]].lat;        //calculating distace between cities
      b = (double)cities[tempTour[i + 1]].lon - (double)cities[tempTour[i]].lon;
      length += sqrt(a*a + b*b);
    }
    a = (double)cities[tempTour[0]].lat - (double)cities[tempTour[numCities - 1]].lat;  //calculating distance from last city to starting city
    b = (double)cities[tempTour[0]].lon - (double)cities[tempTour[numCities - 1]].lon;
    length += sqrt(a*a + b*b);
  }
  void anneal()
  {
    double temperature = 100000000000.0, deltaT = 0.0, tMin = 1.0;  //temperature and probability calculations
    double alpha = .999997, tempLength = 0;
    vector<int> tempTour(numCities);                        //create vector to test new route
    solCount = 0;                                           //calculate how many new tours have been accepted
    randTourSolution();                                     //start with random solution
    calcTourLength(tour, tourLength);                      //length of random solution
    bestLength = tourLength;
    solCount++;                                             //solution added so solCount incremented
    while (temperature > tMin)
    {
      for (int i = 0; i < numCities; i++)
        tempTour[i] = tour[i];
  //    for (int i = 0; i < numCities; i++)
    //  {
        swapNeighbors(tempTour);
        calcTourLength(tempTour, tempLength);
        deltaT = tempLength - tourLength;
        if (deltaT < 0.0 || exp(-(deltaT / temperature)) > ((double)rand() / RAND_MAX))  //if new length is less than old or meets probability
        {
          for (int i = 0; i < numCities; i++)
          {
            tour[i] = tempTour[i];
          }
          tourLength = tempLength;
          solCount++;
          if (solCount % numCities == 0)
          {
            /*for (int i = 0; i < numCities; i++)
            {
              cout << tour[i] << ", ";
            }
            cout <<"\nTour Length: " << tourLength << endl;*/
          }
          if (tourLength < bestLength)
          {
            bestLength = tourLength;
            for (int i = 0; i < numCities; i++)
            {
              bestTour[i] = tour[i];
            }
          }
  //      }
      }
      temperature *= alpha;
    }
    cout << "\nFinal tour = ";
    for (int i = 0; i < numCities; i++)
    {
      cout << bestTour[i] << ", ";
    }
    cout << "\nFinal tour length: " << bestLength << endl;
    ofstream outFile(outFileName.c_str());
    for (int i = 0; i < numCities; i++)
    {
      outFile << bestTour[i] << " ";
    }
    outFile << "\nFinal tour length: " << bestLength << "\n";
    outFile.close();
  }
  annealing(const string fileName)
  {
    int c, x, y;
    city temp;
    ifstream file(fileName.c_str());
    if (file.is_open())
    {
      while(!file.eof())
      {
        file >> c >> x >> y;
        temp.cityNum = c;
        temp.lat = x;
        temp.lon = y;
        cities.push_back(temp);
        tour.push_back(c);
        bestTour.push_back(c);
      }
    }

    outFileName = fileName + ".tour";
    tour.pop_back();
    cities.pop_back();
    numCities = tour.size();
    solCount = 0;
    tourLength = 0;
  }
};

int main(int argc, char *argv[])
{
  srand(unsigned(time(NULL)));
  if (argc != 2){
    cout << "You must specify a file in the command line." << endl;
    return 1;
  }
  const string fileName(argv[1]);
  annealing TSP(fileName);
  TSP.anneal();
  return 0;
}
