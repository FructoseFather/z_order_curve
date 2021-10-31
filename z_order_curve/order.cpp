#include "order.h"

// ********************* Elem class methods *********************

Elem::Elem(unsigned int x_, unsigned int y_, int id_)
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

unsigned int Elem::get_x()
{
    return x;
}

unsigned int Elem::get_y()
{
    return y;
}

// ********************* Order class methods *********************

Order::Order()
{
    elements = 0;
}

Order::Order(vector<tuple<unsigned int, unsigned int, int>> coords)
{
    elements = 0;

    for (tuple<unsigned int, unsigned int, int> tpl : coords)
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

vector<int> Order::find(unsigned int x, unsigned int y, int range)
{
    // check if searched element is out of min/max bounds

    if (!ordered)
    {
        order_curve();
    }
    vector<int> result;

    unsigned long long search = 0;
    calculate_z_val(x, y, &search);

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

void Order::insert(tuple<unsigned int, unsigned int, int> tpl)
{
    ordered = false;

    elements++;

    Elem *e = new Elem(get<0>(tpl), get<1>(tpl), get<2>(tpl));

    curve.push_back(e);
}

void Order::order_curve()
{
    // first set all z values to the elements
    unsigned long long l;

    for (Elem *e : curve)
    {
        calculate_z_val(e->get_x(), e->get_y(), &l);
        e->set_z_val(l);
        l = 0;
    }

    // next sort vector accoring to z values
    sort(curve.begin(), curve.end(),
         [](Elem *e1, Elem *e2)
         {
             return e1->get_z_val() < e2->get_z_val();
         });

    ordered = true; // set flag for ordered
}

void Order::print_order()
{
    for (Elem *e : curve)
    {
        cout << "x: " << e->get_x() << ", y: " << e->get_y() << ", z: " << e->get_z_val() << ", id: " << e->get_origin_id() << endl;
    }
}

int Order::get_id_of_index(int index)
{
    return curve[index]->get_origin_id();
}

unsigned long long Order::get_zval_of_index(int index)
{
    return curve[index]->get_z_val();
}

int Order::size()
{
    return elements;
}

// ********************* Other Methods *********************

void calculate_z_val(unsigned int x, unsigned int y, unsigned long long *z)
{
    static const unsigned long BITMASK[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static const unsigned long BITSHIFTS[] = {1, 2, 4, 8};

    x = (x | (x << BITSHIFTS[3])) & BITMASK[3];
    x = (x | (x << BITSHIFTS[2])) & BITMASK[2];
    x = (x | (x << BITSHIFTS[1])) & BITMASK[1];
    x = (x | (x << BITSHIFTS[0])) & BITMASK[0];

    y = (y | (y << BITSHIFTS[3])) & BITMASK[3];
    y = (y | (y << BITSHIFTS[2])) & BITMASK[2];
    y = (y | (y << BITSHIFTS[1])) & BITMASK[1];
    y = (y | (y << BITSHIFTS[0])) & BITMASK[0];

    *z = x | (y << 1);
}
