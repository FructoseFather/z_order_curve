#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <tuple>
#include <algorithm>

using namespace std;

class Elem
{
    double x, y;              // coordinates
    unsigned long long z_val; // z coordinate / value
    int origin_id;            // id of node

public:
    /**
     * Constructor
     * @param double x coordinate
     * @param double y coordinate
     * @param int id of node 
     */
    Elem(double, double, int);

    /**
     * sets z value to some element
     * @param unsigned long long z value 
     */
    void set_z_val(unsigned long long);

    /**
     * Returns z value of some element
     * @return unsigned long long z value
     */
    unsigned long long get_z_val();

    /**
     * Returns id of some element
     * @return int id
     */
    int get_origin_id();

    /**
     * Returns x coordinate of some element
     * @return double x coordinate
     */
    double get_x();

    /**
     * Returns y coordinate of some element
     * @return double y coordinate
     */
    double get_y();
};

class Order
{
    double max_x, min_x, max_y, min_y; // min and max of both dimensions
    vector<Elem *> curve;              // the curve itself
    bool normalized;                   // flag if curve is ordered and normalized
    int elements;                      // # elements in curve

public:
    /**
     * Constructor 
     * @param vector of tuples with type <double x, double y, int id>
     */
    Order(vector<tuple<double, double, int>>);

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
    int binary_search(int, int, unsigned long long *);

    /**
     * Method for finding the m nearest elements to some element (calls binary_search())
     * Time complexity O(n log n) if normalized=false, O(log n) otherwise
     * @param double x coordinate of searched element
     * @param double y coordinate of searched element
     * @param int size of m (# nearest elements)
     * @return vector of int origin ids of the nextmost elements
     */
    vector<int> find(double, double, int);

    /**
     * Inserts an element into the curve 
     * (lazy, ordering and normalization is done by next find() call)
     * Time complexity O(1)
     * @param tuple of type <double x, double y, int id>
     */
    void insert(tuple<double, double, int>);

    /**
     * Normalizes and sorts all elements of z order curve accdording to their z value
     */
    void order_curve();

    /**
     * Prints all elements of the curve to the console
     */
    void print_order();
};

/**
 * Calculates the z value according to x and y coordinates
 * @param unsigned long long x coordinate
 * @param unsigned long long y coordinate
 * @param pointer to unsigned long long z coordinate
 */
void calculate_z_val_recursion(unsigned long, unsigned long, unsigned long long *);

/**
 * Calculates the z value according to x and y coordinates. 
 * Values are first normalized and then recursive method is called
 * @param double max x coordinate of z order curve
 * @param double min x coordinate of z order curve
 * @param double max y coordinate of z order curve
 * @param double min y coordinate of z order curve
 * @param double x coordinate of element
 * @param double y coordinate of element
 * @param pointer to unsigned long long z coordinate
 */
void calculate_z_val(double, double, double, double, double, double, unsigned long long *);

/**
 * Transforms a double into an unsigned long
 * Mantissa and Exponent are used in their binary representation
 * @param double value
 * @return unsigned long transformed value
 */
unsigned long get_mantissa(double);

/**
 * Normalizes x and y coordinates according to their min and max in the curve
 * @param double max x coordinate of z order curve
 * @param double min x coordinate of z order curve
 * @param double max y coordinate of z order curve
 * @param double min y coordinate of z order curve
 * @param double x coordinate that has to be normalized
 * @param double y coordinate that has to be normalized
 * @return pair of type <double x, double y>
 */
pair<double, double> normalize(double, double, double, double, double, double);

#endif