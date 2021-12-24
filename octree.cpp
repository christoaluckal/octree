#include<iostream>
#include<math.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
// Division Fraction number. Usually half hence 2
#define FRACTION 2

// Max size of each block in metres.
double MAX_SIZE_X = 1;
double MAX_SIZE_Y = 1;
double MAX_SIZE_Z = 1;

// Custom definition for block coords
using block_params = std::vector<double>;

// 19.199452, 72.837440
// 19.198115, 72.839239

// How many metres are present per degree change in latitude and longitude
double latitudeFactor{};
double longitudeFactor{};

// Function to set the above params
void setLatLonFactor(double latitude)
{
    latitudeFactor = 111132.954-559.822*cos(2*M_PI*latitude/180)+1.175*cos(4*M_PI*latitude/180);
    longitudeFactor = 111319.488*cos(M_PI*latitude/180);
}

// double latitudeFactor = 111132.954-559.822*cos(2*M_PI*19.199452/180)+1.175*cos(4*M_PI*19.199452/180);
// double longitudeFactor = 111319.488*cos(M_PI*19.199452/180);

// Obstacle class to create and test if blocks are inside obstacles
class CuboidalObstacle{
    public:
    double ox_min=0,oy_min=0,oz_min=0,ox_max=0,oy_max=0,oz_max=0;
    CuboidalObstacle(double x1,double y1,double z1,double x2,double y2,double z2){
        ox_min = x1;
        oy_min = y1;
        oz_min = z1;
        ox_max = x2;
        oy_max = y2;
        oz_max = z2;
    };
    bool isInObstacle(double x1,double y1,double z1,double x2,double y2,double z2)
    {
        return (x1 >= ox_min && x1<= ox_max && y1 >= oy_min && y1<= oy_max && z1 >= oz_min && z1<= oz_max && 
        x2 >= ox_min && x2<= ox_max && y2 >= oy_min && y2<= oy_max && z2 >= oz_min && z2<= oz_max);
        // return false;
    };
};

// Vector of cuboidal obstacles. IDK how to have cylindrical ones in one data structure.
std::vector<CuboidalObstacle> cuboidal_vec{};

class Block{
    public:
    // For counting children
    int child_count = 0;
    // Initialize bounds of the block to 0
    double x_min=0,y_min=0,x_max=0,y_max=0,mid_x=0,mid_y = 0,z_min=0,z_max=0,mid_z=0;
    // If block lies completely inside obstacle. TODO need to address this.
    bool obstacle = false;
    // Unused pointers. Waiting to figure out.
    Block *leftSibling=nullptr;
    Block *rightSibling=nullptr;

    // Parent pointer for traversal
    Block *parent=nullptr;

    // Children pointer
    Block *child0=nullptr,*child1=nullptr,*child2=nullptr,*child3=nullptr;
    Block *child4=nullptr,*child5=nullptr,*child6=nullptr,*child7=nullptr;
    
    // Vector to get block params from external blocks
    block_params bp{};
    
    // Blank
    Block(){x_min=-1;y_min=-1;x_max=-1;y_max=-1;z_min=-1;z_max=-1;}
    
    // Function to get params
    block_params GetBlockParams(){
        bp.push_back(x_min);
        bp.push_back(y_min);
        bp.push_back(z_min);
        bp.push_back(x_max);
        bp.push_back(y_max);
        bp.push_back(z_max);
        return bp;
    }
    // Function to get number of children
    int getChildCount(){
        if(child0!=nullptr)
        {
            child_count++;
        }
        if(child1!=nullptr)
        {
            child_count++;
        }
        if(child2!=nullptr)
        {
            child_count++;
        }
        if(child3!=nullptr)
        {
            child_count++;
        }
        if(child4!=nullptr)
        {
            child_count++;
        }
        if(child5!=nullptr)
        {
            child_count++;
        }
        if(child6!=nullptr)
        {
            child_count++;
        }
        if(child7!=nullptr)
        {
            child_count++;
        }
        return child_count;
    }
    
    // Main constructor
    Block(double cx_min, double cy_min, double cz_min, double cx_max, double cy_max,double cz_max, Block *cparent)
    {
        x_min=cx_min;
        x_max=cx_max;
        y_min=cy_min;
        y_max=cy_max;
        z_min = cz_min;
        z_max = cz_max;

        // Splitting is done in halves 
        mid_x = (x_max+x_min)/FRACTION;
        mid_y = (y_max+y_min)/FRACTION;
        mid_z = (z_max+z_min)/FRACTION;

        parent = cparent;

        // Iterate through every obstacle and check if current block lies inside. There may be a better way to chece
        for(int i=0;i<cuboidal_vec.size();i++)
        {
            if(cuboidal_vec[i].isInObstacle(x_min,y_min,z_min,x_max,y_max,z_max))
            {
                obstacle = true;
                // std::cout << x_min<<','<<y_min<<','<<z_min<<'|'<<x_max<<','<<y_max<<','<<z_max<<" is in obstacle SKIPPING\n";
            }
            else{
                // std::cout << x_min<<','<<y_min<<','<<z_min<<'|'<<x_max<<','<<y_max<<','<<z_max<<" "<< this << '\n';


                // lower octants            upper octants
                // | 0   |  1 |             |  4 | 5  |
                // | 2   |  3 |             |  6 | 7  |


                if(mid_x-x_min >=MAX_SIZE_X)
                {
                    if(mid_y-y_min >=MAX_SIZE_Y)
                    {
                        if(mid_z-z_min>=MAX_SIZE_Z)
                        {
                            // No Limitation
                            child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                            child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z,this);
                            child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z,this);
                            child3 = new Block(mid_x,mid_y,z_min,x_max,y_max,mid_z,this);
                            child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max,this);
                            child5 = new Block(mid_x,y_min,mid_z,x_max,mid_y,z_max,this);
                            child6 = new Block(x_min,mid_y,mid_z,mid_x,y_max,z_max,this);
                            child7 = new Block(mid_x,mid_y,mid_z,x_max,y_max,z_max,this);
                        }
                        else{
                            // Limited in Z direction (only lower octants)
                            child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                            child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z,this);
                            child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z,this);
                            child3 = new Block(mid_x,mid_y,z_min,x_max,y_max,mid_z,this);
                        }
                    }
                else
                {
                if(mid_z-z_min>=MAX_SIZE_Z)
                    {
                        // Limited in Y direction
                        child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                        child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z,this);
                        child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max,this);
                        child5 = new Block(mid_x,y_min,mid_z,x_max,mid_y,z_max,this);
                    }
                    else{
                        // Limited in YZ direction
                        child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                        child1 = new Block(mid_x,y_min,z_min,x_max,mid_y,mid_z,this);
                    }
                }     
                }
                else
                {
                    if(mid_y-y_min >=MAX_SIZE_Y)
                    {
                        if(mid_z-z_min>=MAX_SIZE_Y)
                        {
                            // Limited in X direction
                            child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                            child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z,this);
                            child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max,this);
                            child6 = new Block(x_min,mid_y,mid_z,mid_x,y_max,z_max,this);
                        }
                        else{
                            // Limited in XZ direction
                            child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                            child2 = new Block(x_min,mid_y,z_min,mid_x,y_max,mid_z,this);
                        }
                    }
                    else
                    {
                    if(mid_z-z_min>=MAX_SIZE_Z)
                        {
                            // Limited in XY direction
                            child0 = new Block(x_min,y_min,z_min,mid_x,mid_y,mid_z,this);
                            child4 = new Block(x_min,y_min,mid_z,mid_x,mid_y,z_max,this);
                        }
                        else{
                        }
                    }     
                }
            }
        }  
    }
};


int printTreeCenters(Block *tree,int count)
{
    // std::cout << counter << '\n';
    if(tree!=nullptr)
    {
        // If obstacle flag is set
        if(!tree->obstacle)
        {
        std::cout << tree->x_min << ',' << tree->y_min << ',' << tree->z_min << '|' << tree->x_max << ',' << tree->y_max << ',' << tree->z_max << '\n';
        // std::cout << tree->getChildCount() << '\n'; 
        count++;
        }

        count = printTreeCenters(tree->child0,count);
        count = printTreeCenters(tree->child1,count);
        count = printTreeCenters(tree->child2,count);
        count = printTreeCenters(tree->child3,count);
        count = printTreeCenters(tree->child4,count);
        count = printTreeCenters(tree->child5,count);
        count = printTreeCenters(tree->child6,count);
        count = printTreeCenters(tree->child7,count);
        // std::cout << tree->mid_x << ',' << tree->mid_y << ',' << tree->mid_z << '\n';    
    }
    // std::cout << count << '\n';
    return count;
}



// 19.199452, 72.837440
// 19.198115, 72.839239

int main(){
    // Get the value per degree change
    setLatLonFactor(19.199452);

    // Basic Obstacle
    CuboidalObstacle obs1 = CuboidalObstacle(50,50,50,200,200,200);
    cuboidal_vec.push_back(obs1);


    double start_x,start_y,end_x,end_y,start_z,end_z,delta_x,delta_y,delta_z;
    int counter = 0;

    // BASIC 
    // start_x = 0;
    // start_y = 0;
    // start_z = 0;
    // end_x = 3;
    // end_y = 3;
    // end_z = 3;

    // INTERMEDIATE
    // Since TL point latitude is higher than BR point latitude, negative is taken so that start < end
    // start_x = -19.199452;
    // start_y = 72.837440;
    // start_z = 0;
    // end_x = -19.198115;
    // end_y = 72.839239;
    // end_z = 100;

    // ADVANCED
    start_x = -19.199452;
    start_y = 72.837440;
    start_z = 0;
    end_x = -19.198115;
    end_y = 72.839239;
    end_z = 100;
    delta_x = end_x-start_x;
    delta_y = end_y-start_y;
    delta_z = end_z-start_z;
    MAX_SIZE_X = 1/longitudeFactor;
    MAX_SIZE_Y = 1/latitudeFactor;

    Block test = Block(start_x,start_y,start_z,end_x,end_y,end_z,nullptr);
    Block *iter = &test;
    int total = printTreeCenters(iter,counter);
    std::cout << "COUNT:" << total << '\n';




}