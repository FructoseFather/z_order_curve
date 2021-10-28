#include "order.h"

// ********************* Elem class methods *********************

Elem::Elem(double x_, double y_, int id_)
{
    origin_id = id_;
    x = x_;
    y = y_;
    z_val = 0;
}

void Elem::set_z_val(unsigned long long z_val_)
{
    z_val = z_val_;
}

unsigned long long Elem::get_z_val()
{
    return z_val;
}

int Elem::get_origin_id()
{
    return origin_id;
}

double Elem::get_x()
{
    return x;
}

double Elem::get_y()
{
    return y;
}

// ********************* Order class methods *********************

Order::Order(vector<tuple<double, double, int>> coords)
{
    min_x = 1;
    max_x = -1;
    min_y = 1;
    max_y = -1;
    elements = 0;

    for (tuple<double, double, int> tpl : coords)
    {
        insert(tpl);
    }
}

Order::~Order()
{
    for (int i = 0; i < elements; i++)
    {
        delete curve[i];
    }
}

int Order::binary_search(int left, int right, unsigned long long *l)
{
    if (right >= left)
    {
        int mid = left + (right - left) / 2;

        if (curve[mid]->get_z_val() == *l)
            return mid;

        if (curve[mid]->get_z_val() > *l)
            return binary_search(left, mid - 1, l);

        return binary_search(mid + 1, right, l);
    }

    // if leftmost element is next return curve[0]
    if (left == 0)
    {
        return 0;
    }
    // if rightmost element is next to searched element return curve[elements-1]
    if (left == elements)
    {
        return elements - 1;
    }
    // else check which element of the two neighbours is closer to searched element
    if (llabs(curve[left]->get_z_val() - *l) < llabs(curve[right]->get_z_val() - *l))
    {
        return left;
    }
    else
    {
        return right;
    }

    return -1;
}

vector<int> Order::find(double x, double y, int range)
{
    // check if searched element is out of min/max bounds
    if (x < min_x)
        min_x = x;
    if (x > max_x)
        max_x = x;
    if (y < min_y)
        min_y = y;
    if (y > max_y)
        max_y = y;

    if (!normalized)
    {
        order_curve();
    }
    vector<int> result;

    unsigned long long search = 0;

    pair<double, double> input = normalize(max_x, min_x, max_y, min_y, x, y);

    calculate_z_val_recursion(get_mantissa(input.first), get_mantissa(input.second), &search);

    search = search >> 1;

    // start binary search
    int res = binary_search(0, elements - 1, &search);

    // left and right boundary of neighbours of nearest element
    int left = res;
    int right = res;

    if ((elements - range) >= 0)
    {
        while (range--)
        {
            if (left == right)
            {
                result.push_back(curve[res]->get_origin_id());
                if (left != 0)
                {
                    left--;
                }
                if (right != (elements - 1))
                {
                    right++;
                }
            }
            else
            {
                if ((left == 0) || (llabs(curve[left]->get_z_val() - search) > llabs(curve[right]->get_z_val() - search)))
                {
                    result.push_back(curve[right]->get_origin_id());
                    right++;
                }
                else if ((right == (elements - 1)) || (llabs(curve[left]->get_z_val() - search) < llabs(curve[right]->get_z_val() - search)))
                {
                    result.push_back(curve[left]->get_origin_id());
                    left--;
                }
            }
        }
    }

    return result;
}

void Order::insert(tuple<double, double, int> tpl)
{
    if (get<0>(tpl) < min_x)
        min_x = get<0>(tpl);
    if (get<0>(tpl) > max_x)
        max_x = get<0>(tpl);
    if (get<1>(tpl) < min_y)
        min_y = get<1>(tpl);
    if (get<1>(tpl) > max_y)
        max_y = get<1>(tpl);

    normalized = false;

    elements++;

    Elem *e = new Elem(get<0>(tpl), get<1>(tpl), get<2>(tpl));

    curve.push_back(e);
}

void Order::order_curve()
{
    // in order to avoid a collision when transforming to binary after normalization, increase max by 1
    max_x++;
    max_y++;
    min_x--;
    min_y--;

    // first set all z values to the elements
    pair<double, double> tmp;
    unsigned long long l;

    for (Elem *e : curve)
    {
        tmp = normalize(max_x, min_x, max_y, min_y, e->get_x(), e->get_y());
        calculate_z_val_recursion(get_mantissa(tmp.first), get_mantissa(tmp.second), &l);
        e->set_z_val(l);
    }

    // next sort vector accoring to z values
    sort(curve.begin(), curve.end(),
         [](Elem *e1, Elem *e2)
         {
             return e1->get_z_val() < e2->get_z_val();
         });
}

void Order::print_order()
{
    for (Elem *e : curve)
    {
        cout << "x: " << e->get_x() << ", y: " << e->get_y() << ", z: " << e->get_z_val() << ", id: " << e->get_origin_id() << endl;
    }
}

// ********************* Other Methods *********************

void calculate_z_val_recursion(unsigned long x, unsigned long y, unsigned long long *z)
{
    if (x || y)
    {
        calculate_z_val_recursion(x >> 1, y >> 1, z);
        if (x % 2)
            *z = *z + 1;
        *z = *z << 1;
        if (y % 2)
            *z = *z + 1;
        *z = *z << 1;
    }
    else
    {
        *z = 0;
    }
    return;
}

void calculate_z_val(double max_x, double min_x, double max_y, double min_y, double x, double y, unsigned long long *z)
{
    pair<double, double> input = normalize(max_x, min_x, max_y, min_y, x, y);

    calculate_z_val_recursion(get_mantissa(input.first), get_mantissa(input.second), z);

    *z = *z >> 1;
}

unsigned long get_mantissa(double val)
{
    int exp;
    unsigned long result = 0;
    double mantissa = frexp(val, &exp);
    int remaining = sizeof(unsigned long) * 8 + exp;

    //cout << "exp: " << exp << ", val: " << val << ", remaining: " << remaining << endl;

    while (--remaining)
    {
        mantissa *= 2;
        if (mantissa >= 1)
        {
            result++;
            mantissa--;
        }
        result = result << 1;
    }

    return result;
}

pair<double, double> normalize(double max_x, double min_x, double max_y, double min_y, double x, double y)
{
    double x_norm, y_norm;

    pair<double, double> result;
    result.first = (x - min_x) / (max_x - min_x);
    result.second = (y - min_y) / (max_y - min_y);

    return result;
}