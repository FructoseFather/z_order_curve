#include "order.h"

// ********************* Elem class methods *********************

Elem::Elem(unsigned int x_, unsigned int y_, int id_)
{
    origin_id = id_;
    x = x_;
    y = y_;
    z_val = 0;
}

void Elem::set_z_val(unsigned long z_val_)
{
    z_val = z_val_;
}

unsigned long Elem::get_z_val()
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
    min=UINT_MAX;
    max=0;
    elements = 0;
}

Order::Order(vector<tuple<unsigned int, unsigned int, int>> coords) : Order()
{
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

int Order::binary_search(int left, int right, unsigned long *l)
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

    unsigned long search = 0;
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
    if(get<0>(tpl)<min)min=get<0>(tpl);
    if(get<0>(tpl)>max)max=get<0>(tpl);
    if(get<1>(tpl)<min)min=get<1>(tpl);
    if(get<1>(tpl)>max)max=get<1>(tpl);

    elements++;

    Elem *e = new Elem(get<0>(tpl), get<1>(tpl), get<2>(tpl));

    curve.push_back(e);
}

void Order::order_curve()
{
    // first set all z values to the elements
    unsigned long l;
    max += 1;

    for (Elem *e : curve)
    {
        unsigned int xn = getMantissa(normalize(max, min, e->get_x()));
        unsigned int yn = getMantissa(normalize(max, min, e->get_y()));
        calculate_z_val(xn,yn,&l);

        string binx = bitset<32>(xn).to_string();
        string biny = bitset<32>(yn).to_string();
        string binl = bitset<64>(l).to_string();

        //cout << "x binary: " << binx << ", y binary: " << biny << ", l binary: " << binl << endl;

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

unsigned long Order::get_zval_of_index(int index)
{
    return curve[index]->get_z_val();
}

int Order::size()
{
    return elements;
}

// ********************* Other Methods *********************

/* void calculate_z_val(unsigned int x, unsigned int y, unsigned long *z)
{
    static const unsigned int BITMASK[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static const unsigned int BITSHIFTS[] = {1, 2, 4, 8};

    x = (x | (x << BITSHIFTS[3])) & BITMASK[3];
    x = (x | (x << BITSHIFTS[2])) & BITMASK[2];
    x = (x | (x << BITSHIFTS[1])) & BITMASK[1];
    x = (x | (x << BITSHIFTS[0])) & BITMASK[0];

    y = (y | (y << BITSHIFTS[3])) & BITMASK[3];
    y = (y | (y << BITSHIFTS[2])) & BITMASK[2];
    y = (y | (y << BITSHIFTS[1])) & BITMASK[1];
    y = (y | (y << BITSHIFTS[0])) & BITMASK[0];

    *z = x | (y << 1);
} */

void calculate_z_val_recursion(unsigned int x, unsigned int y, unsigned long *z)
{
    if (x || y)
    {
        calculate_z_val_recursion(x >> 1, y >> 1, z);
        if (y % 2)
            *z = *z + 1;
        *z = *z << 1;
        if (x % 2)
            *z = *z + 1;
        *z = *z << 1;
    }
    else
    {
        *z = 0;
    }
}

void calculate_z_val(unsigned int x, unsigned int y, unsigned long *z)
{
    calculate_z_val_recursion(x,y, z);

    *z = *z >> 1;

    unsigned long a = 1;
    a = a << 63;
    if (y>>31) *z += a;
}

double normalize( unsigned int max, unsigned int min, unsigned int x){
    return ((double)(x-min))/((double)(max-min));
}


unsigned int getMantissa(double v){
    int exp;
	double mantissa = frexp(v,&exp);

    //cout << v << " = " << mantissa << " * 2^" << exp << endl;

    unsigned int result = 0;

	int remaining = sizeof(unsigned int)*8 + exp;
	//cout << remaining << " bits remaning" << endl;

	while (--remaining) {

		mantissa *= 2;
		if (mantissa >= 1) {
			result ++;
			mantissa --;
		}
		result = result << 1;
	}

	//cout << "result: " << result << endl;

	//string bin = bitset<64>(result).to_string();

	//cout << "result binary: " << bin << endl;

    return result;
}
	

	
