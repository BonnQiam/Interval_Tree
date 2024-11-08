#ifndef INTERVAL_TREE_H
#define INTERVAL_TREE_H

#include <iostream>
#include <cmath>

#include <vector>

struct Interval {
    int start, end;
};

struct IntervalTreeNode {
    double value;
    std::vector<Interval> Left_Endpoints;// sort by increasing order
    std::vector<Interval> Right_Endpoints;// sort by increasing order
    //int Left_active_child, Right_active_child;
};

/*
 * The primary structure of the interval tree is a Complete Binary Tree. We implement the tree using vector.
*/

class IntervalTree {

private:
    std::vector<IntervalTreeNode> CBT;// Using the Complete Binary Tree to store the Node, the leaf nodes are the endpoints of all segments

public:
    IntervalTree(std::vector<int> Seg_Endpoints) {
        int TreeSize = 2 * Seg_Endpoints.size()-1;
        CBT.resize( TreeSize + 1);
        CBT[0].value = 0;// Do not use the 0th index

        // Calculate the depth of the tree
        int Depth = log2(Seg_Endpoints.size()) + 1;
        int Index_1 = pow(2, Depth);// The left-most index of the last level
        int Index_2 = Index_1 - (Seg_Endpoints.size() - (TreeSize - Index_1))+1; // The left-most index of the leaf node in the last full level

        
        // Assign the leaf nodes
        for(int i = Index_2; i < CBT.size(); i++) {
            if(i < Index_1) {
                CBT[i].value = Seg_Endpoints[ (TreeSize - Index_1) + 1 +  i - Index_2];
            }
            else {
                CBT[i].value = Seg_Endpoints[i - Index_1];
            }
        }

        // Assign the non-leaf nodes
        for(int i = Index_2 - 1; i > 0; i--) {
            CBT[i].value = (CBT[2*i].value + CBT[2*i + 1].value) / 2;
        }
    }

    void Traverse_Display(int index){
        if(index >= CBT.size()) {
            return;
        }

        std::cout << "CBT[" << index << "]: " << CBT[index].value << std::endl;

        for(auto &Interval : CBT[index].Left_Endpoints) {
            std::cout << "Left_Endpoints: " << Interval.start << std::endl;
        }

        for(auto &Interval : CBT[index].Right_Endpoints) {
            std::cout << "Right_Endpoints: " << " " << Interval.end << std::endl;
        }

        std::cout << "----------------------" << std::endl;

        Traverse_Display(2 * index);
        Traverse_Display(2 * index + 1);
    }

    std::vector<Interval> Traverse_Collect_Interval(int index){
        std::vector<Interval> Result;

        if(index >= CBT.size()) {
            return Result;
        }

        for(auto &Interval : CBT[index].Left_Endpoints) {
            Result.push_back(Interval);
        }

        std::vector<Interval> Left_Result = Traverse_Collect_Interval(2 * index);
        std::vector<Interval> Right_Result = Traverse_Collect_Interval(2 * index + 1);

        Result.insert(Result.end(), Left_Result.begin(), Left_Result.end());
        Result.insert(Result.end(), Right_Result.begin(), Right_Result.end());

        return Result;
    }

    void Insert(Interval I) {
        int index = 1;

        while( (CBT[index].value > I.end || CBT[index].value < I.start) && index < CBT.size()) {
            if(CBT[index].value > I.end) {
                index = 2 * index;
            }
            else {
                index = 2 * index + 1;
            }
        }

        if(index >= CBT.size()) {
            //std::cout << "The interval is out of range" << std::endl;
            return;
        }

        // Insert I.start into the left endpoint list by increasing order
        auto itr = CBT[index].Left_Endpoints.begin();
        while(itr != CBT[index].Left_Endpoints.end() && itr->start < I.start) {
            itr++;
        }
        CBT[index].Left_Endpoints.insert(itr, I);

        // Insert I.end into the right endpoint list by decreasing order
        itr = CBT[index].Right_Endpoints.begin();
        while(itr != CBT[index].Right_Endpoints.end() && itr->end > I.end) {
            itr++;
        }
        CBT[index].Right_Endpoints.insert(itr, I);

        // Update the active child

    }

    void Delete(Interval I) {
        int index = 1;

        while( (CBT[index].value > I.end || CBT[index].value < I.start) && index < CBT.size()) {
            if(CBT[index].value > I.end) {
                index = 2 * index;
            }
            else {
                index = 2 * index + 1;
            }
        }

        if(index >= CBT.size()) {
            //std::cout << "The interval is out of range" << std::endl;
            return;
        }

        // Delete I.start from the left endpoint list
        auto itr = CBT[index].Left_Endpoints.begin();
        while(itr != CBT[index].Left_Endpoints.end() && itr->start != I.start) {
            itr++;
        }
        if(itr != CBT[index].Left_Endpoints.end()) {
            CBT[index].Left_Endpoints.erase(itr);
        }

        // Delete I.end from the right endpoint list
        itr = CBT[index].Right_Endpoints.begin();
        while(itr != CBT[index].Right_Endpoints.end() && itr->end != I.end) {
            itr++;
        }
        if(itr != CBT[index].Right_Endpoints.end()) {
            CBT[index].Right_Endpoints.erase(itr);
        }
    }

#if 1
    std::vector<Interval> Overlap_locate_ST(int index, int ST){
        std::vector<Interval> Result;
        
        if(index >= CBT.size()) {
            return Result;
        }

        if(CBT[index].value == ST){
            return Result;
        }

        int Flag = 0;

        if(CBT[index].value > ST) {
            for(auto &Interval : CBT[index].Left_Endpoints) 
                Result.push_back(Interval);

            std::vector<Interval> Right_Result = Traverse_Collect_Interval(2 * index + 1);
            Result.insert(Result.end(), Right_Result.begin(), Right_Result.end());
            Flag = 0; // check in left child
        }
        else {
            for(auto &Interval : CBT[index].Right_Endpoints) {
                if(Interval.end >= ST) {
                    Result.push_back(Interval);
                }
            }

            Flag = 1; // check in right child
        }

        std::vector<Interval> Temp_Result;
        if(Flag == 0){
            Temp_Result = Overlap_locate_ST(2 * index, ST);
        }
        else {
            Temp_Result = Overlap_locate_ST(2 * index + 1, ST);
        }
        
        Result.insert(Result.end(), Temp_Result.begin(), Temp_Result.end());

        return Result;
    }

    std::vector<Interval> Overlap_locate_ED(int index, int ED){
        std::vector<Interval> Result;

        if(index >= CBT.size()) {
            return Result;
        }

        if(CBT[index].value == ED){
            return Result;
        }

        int Flag = 0;

        if(CBT[index].value > ED) {
            for(auto &Interval : CBT[index].Left_Endpoints) {
                if(Interval.start <= ED) {
                    Result.push_back(Interval);
                }
            }

            Flag = 0; // check in left child
        }
        else {
            for(auto &Interval : CBT[index].Right_Endpoints) 
                Result.push_back(Interval);

            std::vector<Interval> Left_Result = Traverse_Collect_Interval(2 * index);
            Result.insert(Result.end(), Left_Result.begin(), Left_Result.end());
            Flag = 1; // check in right child
        }

        std::vector<Interval> Temp_Result;

        if(Flag == 0){
            Temp_Result = Overlap_locate_ED(2 * index, ED);
        }
        else {
            Temp_Result = Overlap_locate_ED(2 * index + 1, ED);
        }

        Result.insert(Result.end(), Temp_Result.begin(), Temp_Result.end());

        return Result;
    }

    //! Requirement: [ST, ED] is the interval that has been inserted into the tree
    //* Return the intervals that overlap with [ST, ED]
    std::vector<Interval> Overlap_Query(int ST, int ED) {
        std::vector<Interval> Result;

        int index = 1;
        /*
        * Find the node that satisfies the condition: CBT[index].value >= ST && CBT[index].value <= ED
        * Names the node as the target node
        */
        while((CBT[index].value > ED || CBT[index].value < ST) ) {
            if(CBT[index].value > ED) {
                for(auto &Interval : CBT[index].Left_Endpoints) {
                    if(Interval.start <= ST) {
                        Result.push_back(Interval);
                    }
                }

                index = 2 * index;
            }
            
            if(CBT[index].value < ST) {
                for(auto &Interval : CBT[index].Right_Endpoints) {
                    if(Interval.end >= ED) {
                        Result.push_back(Interval);
                    }
                }
                index = 2 * index + 1;
            }
        }

        for(auto &Interval : CBT[index].Left_Endpoints) {
            if(Interval.start >= ST && Interval.end <= ED) {
                Result.push_back(Interval);
            }
        }

        std::vector<Interval> Left_Result = Overlap_locate_ST(2*index, ST);
        Result.insert(Result.end(), Left_Result.begin(), Left_Result.end());

        std::vector<Interval> Right_Result = Overlap_locate_ED(2*index+1, ED);
        Result.insert(Result.end(), Right_Result.begin(), Right_Result.end());

        return Result;
    }
    
#endif

};


#endif // !INTERVAL_TREE_H