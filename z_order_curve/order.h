#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <tuple>
#include <algorithm>
#include <limits.h>
#include <bitset>

using namespace std;

class Elem
{
    unsigned int x, y;        // coordinates
    unsigned long z_val; // z coordinate / value
    int origin_id;            // id of node

public:
    /**
     * Constructor
     * @param unsigned int x coordinate
     * @param unsigned int y coordinate
     * @param int id of node 
     */
    Elem(unsigned int, unsigned int, int);

    /**
     * sets z value to this element
     * @param unsigned long long z value 
     */
    void set_z_val(unsigned long);

    /**
     * Returns z value of this element
     * @return unsigned long long z value
     */
    unsigned long get_z_val();

    /**
     * Returns id of this element
     * @return int id
     */
    int get_origin_id();

    /**
     * Returns x coordinate of this element
     * @return unsigned int x coordinate
     */
    unsigned int get_x();

    /**
     * Returns y coordinate of this element
     * @return unsigned int y coordinate
     */
    unsigned int get_y();
};

class Order
{
    vector<Elem *> curve; // the curve itself
    bool ordered;         // flag if curve is ordered
    int elements;         // # elements in curve
    unsigned int min,max;

public:
    /**
     * Default constructor 
     */
    Order();

    /**
     * Constructor 
     * @param vector of tuples with type <unsigned int x, unsigned int y, int id>
     */
    Order(vector<tuple<unsigned int, unsigned int, int>>);

    /**
     * Destructor
     */
    ~Order();

    /**
     * Method to apply binary search on a z order curve
     * Time complexity O(log n)
     * @param int left border of searchable area
     * @param int right border of searchable area
     * @param unsigned long long z value of searched element
     * @return int index of nearest element to searched element
     */
    int binary_search(int, int, unsigned long *);

    /**
     * Method for finding the m nearest elements to some element (calls binary_search())
     * Time complexity O(n log n) if normalized=false, O(log n) otherwise
     * @param unsigned int x coordinate of searched element
     * @param unsigned int y coordinate of searched element
     * @param int size of m (# nearest elements)
     * @return vector of int origin ids of the nextmost elements
     */
    vector<int> find(unsigned int, unsigned int, int);

    /**
     * Inserts an element into the curve 
     * (lazy, ordering is done by next find() call)
     * Time complexity O(1)
     * @param tuple of type <unsigned int x, unsigned int y, int id>
     */
    void insert(tuple<unsigned int, unsigned int, int>);

    /**
     * Normalizes and sorts all elements of z order curve accdording to their z value
     */
    void order_curve();

    /**
     * Prints all elements of the curve to the console
     */
    void print_order();

    /**
     * Returns the id of an index
     * @param int index
     * @return int id 
     */
    int get_id_of_index(int);

    /**
     * Returns the z val of an index
     * @param int index
     * @return unsigned long long z val 
     */
    unsigned long get_zval_of_index(int);

    /**
     * Returns the size of the z order curve
     * @return int size
     */
    int size();
};

void calculate_z_val_recursion(unsigned int x, unsigned int y, unsigned long *z);

void calculate_z_val(unsigned int, unsigned int, unsigned long *);

double normalize(unsigned int, unsigned int, unsigned int);

unsigned int getMantissa(double);


#endif