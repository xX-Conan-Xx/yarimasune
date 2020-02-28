/**
Pizza ordering problem:

Solution: 01-bag DP solution + brunches cutting.

DP function:
f(begin_pizza_id, rest_capacity):
    the optimal solution of problem:
    Choose maximum number of pizza slices
    from the pizza with id within [begin_pizza_id, last one]
    while not exceed the rest_capacity.

The optimal solution of origin problem is:
    f(0, max_pizza_consumption)

and the recursion is:
f(pizza_id, rest_capacity) = max(f(pizza_id + 1, rest_capacity - slices[pizza_id]),
                                 f(pizza_id + 1, rest_capacity))

The slices number of each pizza is inputed in unstrictly increasing order.
Some branch cutting can be carried out.
*/

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

using std::cout;
using std::endl;

int max_cap, type_num, max_ans;

// Data types definition
typedef int PizzaId;
typedef int SliceNum;

typedef std::pair<PizzaId, SliceNum> DataBunchBase;
class DataBunch: public DataBunchBase {
public:
    DataBunch(PizzaId pza=-1, SliceNum rest=-1): DataBunchBase(pza, rest) {}
    inline SliceNum& ans() { return first; }
    inline PizzaId& next() { return second; }
    inline bool recorded() { return ans() >= 0; }
};

typedef std::unordered_map<int, DataBunch> HashAxis;
typedef std::vector<HashAxis> DataMatrix;
typedef HashAxis::iterator DataIter;

DataMatrix mat(0); // Create data matrix but not initialize the hash table inside
std::vector<int> slices, slices_sum_from;

void init_data_matrix();
int cal_result(PizzaId beg, SliceNum rest);
void print_result();

int main(){
    // Input data size
    std::cin >> max_cap >> type_num;
    // Initialize data
    slices.resize(type_num);
    slices_sum_from.resize(type_num);
    // Input each pizza slice
    for(int& n : slices) {
        std::cin >> n;
    }
    // Calculate the partial sum of pizza slices
    slices_sum_from[type_num-1] = slices.back();
    for (int i = type_num-2; i >= 0; --i) {
        slices_sum_from[i] = slices_sum_from[i+1] + slices[i];
    }
    // Initialize the matrix
    init_data_matrix();
    // Main algorithm
    cal_result(0, max_cap);
    // Print solution
    print_result();
    return 0;
}

void init_data_matrix() {
    // It is tested that several simple memory strategies
    // show little difference in effect on overall exec time.
    // and we don't know how to write a complex one.
    // Therefore, just choose the most simply one :)
    mat.resize(type_num);
}

SliceNum cal_result(PizzaId beg, SliceNum rest) {
    // Ending case 1: the pizza id exceed maximum
    if (beg >= type_num) {
        return max_cap-rest;
    }

    // Ending case 2: current pizza size exceeded the remaining space
    // Since the pizza size is stored in increasing order, the remaining pizza will also exceed
    if (slices[beg] > rest) {
        return max_cap-rest;
    }

    // Ending case 3: sum of chosen and all remaining pizza is less than current maximum
    if (max_cap-rest + slices_sum_from[beg] < max_ans) {
        return 0;
    }

    // Ending case 4: answer already recorded
    DataBunch &data = mat[beg][rest];
    if (data.recorded())
        return data.ans();

    // DP recursion
    int ans1 = cal_result(beg+1, rest-slices[beg]), ans2 = 0;

    // Cut branch no more optimal value can be found.
    if (ans1 != max_cap) {
        ans2 = cal_result(beg+1, rest);
    }

    data.ans() = (ans1 > ans2 ? ans1 : ans2);
    data.next() = (ans1 == data.ans() ? beg : mat[beg+1][rest].next());
    if(max_ans < data.ans()) {
        max_ans = data.ans();
    }
    return data.ans();
}


PizzaId get_next(PizzaId id, SliceNum rest);
const int Selected = 0;
void print_result() {
    PizzaId current = mat[0][max_cap].next();
    SliceNum rest = max_cap;
    int counter = 0;
    std::vector<int> &records = slices_sum_from;

    while (current < type_num) {
        rest -= slices[current];
        records[current] = Selected;
        counter++;
        current = get_next(current+1, rest);
    }
    cout << counter << endl;
    for(int i = 0; i < type_num; ++i) {
        if (records[i] == Selected) {
            cout << i << ' ';
        }
    }
    cout << endl;
}

PizzaId get_next(PizzaId beg, SliceNum rest) {
    if (beg >= type_num || slices[beg] > rest) {
        return type_num;
    } else {
        return mat[beg][rest].next();
    }
}
