#include<iostream>
#include<math.h>
#include <chrono>
#include <thread>
#define MAX_SIZE 2
#define FRACTION 2

// 19.199452, 72.837440
// 19.198115, 72.839239

double latitudeFactor = 111132.954-559.822*cos(2*M_PI*19.199452/180)+1.175*cos(4*M_PI*19.199452/180);
double longitudeFactor = 111319.488*cos(M_PI*19.199452/180);


class Block{
    public:
    double x_min=0,y_min=0,x_max=0,y_max=0,mid_x=0,mid_y = 0,z_min=0,z_max=0,mid_z=0;
    double x_extent=0,y_extent=0;
    int leaf = 0;
    Block *leftSibling=nullptr;
    Block *rightSibling=nullptr;
    Block *child0=nullptr,*child1=nullptr,*child2=nullptr,*child3=nullptr;
    Block *child4=nullptr,*child5=nullptr,*child6=nullptr,*child7=nullptr;
    Block(){x_min=-1;y_min=-1;x_max=-1;y_max=-1;z_min=-1;z_max=-1;}
    Block(double cx_min, double cy_min, double cz_min, double cx_max, double cy_max,double cz_max)
    {
        x_min=cx_min;
        x_max=cx_max;
        y_min=cy_min;
        y_max=cy_max;
        z_min = cz_min;
        z_max = cz_max; 
        mid_x = (x_max+x_min)/FRACTION;
        mid_y = (y_max+y_min)/FRACTION;
        mid_z = (z_max+z_min)/FRACTION;
        if(mid_x-x_min > MAX_SIZE)
        {
            if(mid_y-y_min > MAX_SIZE)
            {
                if(mid_z-z_min>MAX_SIZE)
                {
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z);
                    child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z);
                    child3 = new Block(mid_x,mid_y,z_min,x_max,y_max,mid_z);
                    child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max);
                    child5 = new Block(mid_x,y_min,mid_z,x_max,mid_y,z_max);
                    child6 = new Block(x_min,mid_y,mid_z,mid_x,y_max,z_max);
                    child7 = new Block(mid_x,mid_y,mid_z,x_max,y_max,z_max);
                }
                else{
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z);
                    child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z);
                    child3 = new Block(mid_x,mid_y,z_min,x_max,y_max,mid_z);
                }
            }
            else
            {
               if(mid_z-z_min>MAX_SIZE)
                {
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z);
                    child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max);
                    child5 = new Block(mid_x,y_min,mid_z,x_max,mid_y,z_max);
                }
                else{
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z);
                }
            }     
        }
        else
        {
            if(mid_y-y_min >MAX_SIZE)
            {
                if(mid_z-z_min>MAX_SIZE)
                {
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z);
                    child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max);
                    child6 = new Block(x_min,mid_y,mid_z,mid_x,y_max,z_max);
                }
                else{
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z);
                }
            }
            else
            {
               if(mid_z-z_min>MAX_SIZE)
                {
                    child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z);
                    child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max);
                }
                else{
                }
            }     
        }
    }
};

// NODE COUNTER
int counter=0;

void printTree(Block *tree)
{
    if(tree!=NULL)
    {
        std::cout << tree->x_min << ',' << tree->y_min << ',' << tree->z_min << '|' << tree->x_max << ',' << tree->y_max << ',' << tree->z_max << '\n';
        printTree(tree->child0);
        printTree(tree->child1);
        printTree(tree->child2);
        printTree(tree->child3);
        printTree(tree->child4);
        printTree(tree->child5);
        printTree(tree->child6);
        printTree(tree->child7);
    }
    else{
        counter++;
    }
}

int main(){
    double start_x,start_y,end_x,end_y,start_z,end_z;
    start_x = 0;
    start_y = 0;
    start_z = 0;
    end_x = 5;
    end_y = 5;
    end_z = 5;
    Block test = Block(start_x,start_y,start_z,end_x,end_y,end_z);
    // test.getXExtent(test.x_min,test.x_max);
    Block *iter = &test;
    printTree(iter);
    std::cout << "COUNT:" << counter << '\n';
}