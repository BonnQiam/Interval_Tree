#include <algorithm>

#include "Interval_tree.h"

int main() {

    std::vector<Interval> Intervals = {
        {6,36}, {34,38}, {21,36}, {23,27},
        {3,8}, {15,19}, {11,14}
    };

    std::vector<int> Seg_Endpoints;
    for(auto &Interval : Intervals) {
        Seg_Endpoints.push_back(Interval.start);
        Seg_Endpoints.push_back(Interval.end);
    }

    // sort and remove duplicates
    std::sort(Seg_Endpoints.begin(), Seg_Endpoints.end());
    auto itr = std::unique(Seg_Endpoints.begin(), Seg_Endpoints.end());
    Seg_Endpoints.erase(itr, Seg_Endpoints.end());

//    for(auto &Endpoint : Seg_Endpoints) {
//        std::cout << "Endpoint: " << Endpoint << std::endl;
//    }

    IntervalTree Tree(Seg_Endpoints);

//    Tree.Traverse_Display(1);
//    std::cout << "============================================" << std::endl;

    for(auto &Interval : Intervals) {
        Tree.Insert(Interval);
    }

//    Tree.Traverse_Display(1);
//    std::cout << "============================================" << std::endl;
//    Tree.Delete({6,36});
//    Tree.Traverse_Display(1);

    std::vector<Interval> Result = Tree.Overlap_Query(11, 36);

    for(auto &Interval : Result) 
        std::cout << "Interval: " << Interval.start << " " << Interval.end << std::endl;

    return 0;
}