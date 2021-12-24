#include<iostream>
#include<math.h>
#include <chrono>
#include <thread>


// 19.199452, 72.837440
// 19.198115, 72.839239

double latitudeFactor = 111132.954-559.822*cos(2*M_PI*19.199452/180)+1.175*cos(4*M_PI*19.199452/180);
double longitudeFactor = 111319.488*cos(M_PI*19.199452/180);


class Block{
    public:
    double x_min=0,y_min=0,x_max=0,y_max=0,mid_x=0,mid_y = 0;
    double x_extent=0,y_extent=0;
    int leaf = 0;
    Block *leftSibling=nullptr;
    Block *rightSibling=nullptr;
    Block *child0=nullptr,*child1=nullptr,*child2=nullptr,*child3=nullptr;
    // double vertical_extent;
    Block(){x_min=-1;y_min=-1;x_max=-1;y_max=-1;}
    Block(double cx_min, double cy_min, double cx_max, double cy_max)
    {
        x_min=cx_min;
        x_max=cx_max;
        y_min=cy_min;
        y_max=cy_max; 
        // getXExtent(x_min,x_max);
        // getYExtent(y_min,y_max);
        mid_x = (x_max+x_min)/2;
        mid_y = (y_max+y_min)/2;
        // std::cout << this << '\n';
        // std::cout << "xmin:" << x_min << " ymin:" << y_min << " xmax:" << x_max << " ymax:" << y_max << ' ' << mid_x << ' ' << mid_y << '\n';
        if(mid_x-x_min > 1)
        {
            // std::cout << "1 xmin:" << x_min << " ymin:" << y_min << " xmax:" << x_max << " ymax:" << y_max << '\n';
            if(mid_y-y_min > 1)
            {
                // std::cout << "2\n";
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

                child0 = new Block(x_min,y_min,mid_x,mid_y);
                // std::cout << "1 CHILD0 done\n";
                child1 = new Block(mid_x,y_min,x_max,mid_y);
                // std::cout << "1 CHILD1 done\n";
                child2 = new Block(x_min,mid_y,mid_x,y_max);
                // std::cout << "1 CHILD2 done\n";
                child3 = new Block(mid_x,mid_y,x_max,y_max);
                // std::cout << "1 CHILD3 done\n";

            }
            else
            {
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

                // std::cout << "3\n";
                child0 = new Block(x_min,y_min,mid_x,y_max);
                // std::cout << "2 CHILD0 done\n";
                child1 = new Block(mid_x,y_min,x_max,y_max);
                // std::cout << "2 CHILD1 done\n";

            }     
        }
        else
        {
            // std::cout << "2 xmin:" << x_min << " ymin:" << y_min << " xmax:" << x_max << " ymax:" << y_max << '\n';
            // std::cout << "4\n";
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if(mid_y-y_min > 1)
            {
                // std::cout << "5\n";
                // double mid_y = y_extent/2;
                child0 = new Block(x_min,y_min,x_max,mid_y);
                // std::cout << "3 CHILD0 done\n";
                child2 = new Block(x_min,mid_y,x_max,y_max);
                // std::cout << "3 CHILD2 done\n";
            }
            else{
                // std::cout << "4 NONE\n";
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

                leaf = 1;

            }
        }
        // std::cout << "CHILDREN:" << child0 << ' ' << child1 << ' ' << child2 << ' ' << child3 << '\n';

        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Block *child4=nullptr,*child5=nullptr,*child6=nullptr,*child7=nullptr;
    // void getXExtent(double x_min,double x_max){x_extent = x_max-x_min;}
    // void getYExtent(double y_min,double y_max){y_extent = y_max-y_min;}

};

// NODE COUNTER
int counter=0;

void printTree(Block *tree)
{
    if(tree!=NULL)
    {
        std::cout << tree->x_min << ',' << tree->y_min << '|' << tree->x_max << ',' << tree->y_max << '\n';
        printTree(tree->child0);
        printTree(tree->child1);
        printTree(tree->child2);
        printTree(tree->child3);
    }
    else{
        counter++;
    }
}

int main(){
    double start_x,start_y,end_x,end_y;
    start_x = 0;
    start_y = 0;
    end_x = 2;
    end_y = 3;
    Block test = Block(start_x,start_y,end_x,end_y);
    // test.getXExtent(test.x_min,test.x_max);
    Block *iter = &test;
    printTree(iter);
    std::cout << "COUNT:" << counter << '\n';
}