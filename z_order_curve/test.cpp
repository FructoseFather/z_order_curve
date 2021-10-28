#include <iostream>
#include <string>
#include "order.h"
#include <bitset>

using namespace std;

int main(int argc, char *argv[])
{
    vector<tuple<double, double, int>> input;

    input.push_back(make_tuple(7.42, 2.53, 1));
    input.push_back(make_tuple(9.124, 4.12, 2));
    input.push_back(make_tuple(1.64, 3.67, 3));
    input.push_back(make_tuple(20.1, 5.999, 4));
    input.push_back(make_tuple(31.56, 30.4, 5));
    input.push_back(make_tuple(10.51, 10.5312, 6));
    input.push_back(make_tuple(0.51, 0.5312, 7));

    Order curve(input);

    vector<int> res = curve.find(34, 34, 4);

    curve.print_order();

    for (int i : res)
    {
        cout << i << ", ";
    }
    cout << endl;

    return 0;
}
