#include <queue>
/*
heuristhic():
    oclide:
    return X (X - Y)
    Manhattan:
    return X + Y

Node node:
    cost = X + Y

CalcPath():
open-list <- 0
closed-list <- 0

while open-list NOT empty:
    best <- find in open-list the node smallest cost
    open-list.pop(best)
    for i from 0 to 8:


*/
struct Node {
    int x;
    int y;
    bool bIsWalkable;
};
// Manhattan or Oclide
int Heuristic(const Node& n){
    // return cost from a n node to a goal node
    return 1;
}
// cost of reaching a particular node from the start node.
int ActualCost(const Node& n){
    return 1;
}
// determine priority of nodes in the search process
int TotalCost(const Node& n){
    return ActualCost(n) + Heuristic(n);
}
void CalcPath(){
    std::priority_queue<Node> openList;
    std::priority_queue<Node> closedList;
}

int main(){
    return 0;
}