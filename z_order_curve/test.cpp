#include <iostream>
#include <string>
#include "order.h"

using namespace std;

int main(int argc, char *argv[])
{
    vector<tuple<unsigned int, unsigned int, int>> input;

    input.push_back(make_tuple(0, 5, 1));
    input.push_back(make_tuple(1, 4, 2));
    input.push_back(make_tuple(0, 4, 3));
    input.push_back(make_tuple(7, 3, 4));
    input.push_back(make_tuple(6, 3, 5));
    input.push_back(make_tuple(7, 2, 6));
    input.push_back(make_tuple(6, 2, 7));
    input.push_back(make_tuple(5, 3, 8));
    input.push_back(make_tuple(4, 3, 9));
    input.push_back(make_tuple(5, 2, 10));
    input.push_back(make_tuple(4, 2, 11));
    input.push_back(make_tuple(7, 1, 12));
    input.push_back(make_tuple(6, 1, 13));
    input.push_back(make_tuple(7, 0, 14));
    input.push_back(make_tuple(6, 0, 15));
    input.push_back(make_tuple(5, 1, 16));
    input.push_back(make_tuple(4, 1, 17));
    input.push_back(make_tuple(5, 0, 18));
    input.push_back(make_tuple(4, 0, 19));
    input.push_back(make_tuple(3, 3, 20));
    input.push_back(make_tuple(2, 3, 21));
    input.push_back(make_tuple(3, 2, 22));
    input.push_back(make_tuple(2, 2, 23));
    input.push_back(make_tuple(1, 3, 24));
    input.push_back(make_tuple(0, 3, 25));
    input.push_back(make_tuple(1, 2, 26));
    input.push_back(make_tuple(0, 2, 27));
    input.push_back(make_tuple(3, 1, 28));
    input.push_back(make_tuple(2, 1, 29));
    input.push_back(make_tuple(3, 0, 30));
    input.push_back(make_tuple(2, 0, 31));
    input.push_back(make_tuple(1, 1, 32));
    input.push_back(make_tuple(0, 1, 33));
    input.push_back(make_tuple(1, 0, 34));
    input.push_back(make_tuple(0, 0, 35));

    Order curve(input);
    curve.order_curve();

    for (int i = 0; i < curve.size(); i++)
    {
        cout << curve.get_id_of_index(i) << ", ";
    }
    cout << endl;

    return 0;
}
