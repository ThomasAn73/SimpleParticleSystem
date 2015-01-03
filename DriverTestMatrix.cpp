#include "ClassListsMatrix.hpp"
#include "ClassLinkedList.hpp"
#include <iostream>

int main ()
{
    bool test;
    long OneValue;
    TypeListsMatrix CollisionsGrid(40,30);

    CollisionsGrid.AddValueAt(2,1,45);
    CollisionsGrid.AddValueAt(2,1,12);
    CollisionsGrid.AddValueAt(2,1,52);
    CollisionsGrid.AddValueAt(2,1,60);
    std::cout<<"Population of grid [2,1]="<<CollisionsGrid.getCellPopulation(2,1)<<"\n";
    //test=CollisionsGrid.DeleteValueAt(2,1,50);
    //if (test==false) std::cout<<"Could not Delete the desired value\n";
    //std::cout<<"Population of grid [2,1]="<<CollisionsGrid.getCellPopulation(2,1)<<"\n";

    OneValue=CollisionsGrid.getValueFrom(2,1,3); std::cout<<"\n";

    test=CollisionsGrid.AddValueAt(39,29,61);
    if (test==false) std::cout<<"Could not add the desired value\n";
    test=CollisionsGrid.MoveDataValue(2,1,39,30,45);
    if (test==false) std::cout<<"Could not move the desired value\n";

    std::cout<<"Population of grid [2,1]="<<CollisionsGrid.getCellPopulation(2,1)<<"\n";
    std::cout<<"Population of grid [39,29]="<<CollisionsGrid.getCellPopulation(39,29)<<"\n";
    return 0;
}
