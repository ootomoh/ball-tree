#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <vector>

#define SAFE_RELEASE(p) if(p){delete (p); (p) = nullptr;}

const int N = 21;
const int D = 100;

class Point{
	int id;
	std::vector<float> x;
	int dim;
public:
	Point(int id,int dim):id(id),dim(dim),x(std::vector<float>(dim)){}
	Point* init(std::mt19937& mt){
		std::uniform_real_distribution<float> dist(-10.0f,10.0f);
		std::generate(x.begin(),x.end(),[&dist,&mt](){return dist(mt);});
		return this;
	}
	Point* init(float c){
		std::generate(x.begin(),x.end(),[&c](){return c;});
	}
	Point* print(){
		std::cout<<"p["<<id<<"] = (";
		for(int i = 0;i < x.size()-1;i++)printf("%.03f,",x[i]);
		printf("%.03f)\n",x[x.size()-1]);
	}
	int getId(){return id;}
	int getDim(){return dim;}
	Point operator+(const Point& p) const{
		Point sum(-1,dim);
		for(int i = 0;i < dim;i++)
			sum.x[i] = x[i] + p.x[i];
		return sum;
	}
	Point operator-(const Point& p) const{
		Point sum(-1,dim);
		for(int i = 0;i < dim;i++)
			sum.x[i] = x[i] + p.x[i];
		return sum;
	}
	Point operator/(float c) const{
		Point div(-1,dim);
		std::transform(x.begin(),x.end(),div.x.begin(),[&c](float a){return a/c;});
		return div;
	}
	float getSquareLength() const{
		return std::accumulate(x.begin(),x.end(),0.0f,[](float a,float b){return a+b*b;});
	}
	float getX(int n){
		return x[n];
	}

};

class BallTree{
	std::string node_name;
	BallTree *parent,*child0,*child1;
	std::vector<Point> points;
	float radius;
public:
	BallTree(std::vector<Point> points,std::string node_name="root"):points(points),parent(nullptr),child0(nullptr),child1(nullptr),node_name(node_name){}
	BallTree* construct(){
		if(points.size() == 1)return this;
		// 重心計算
		//Point centroid = *Point{-1,points[0].getDim()}.init(0.0f);
		Point centroid(-1,points[0].getDim());centroid.init(0.0f);
		centroid = std::accumulate(points.begin(),points.end(),centroid)/(float)points.size();
		
		// すべてが入る球の半径
		radius = std::sqrt(std::accumulate(points.begin(),points.end(),0.0f,[&centroid](float a,Point b){return std::max(a,(b-centroid).getSquareLength());}));
		int largest_spread_dim = 0;
		float largest_spread = 0.0f;
		for(int i = 0;i < points[0].getDim();i++){
			float max = -10.0f,min = 10.0f;
			float spread = 0.0f;
			for(Point p : points){
				max = std::max(p.getX(i),max);
				min = std::min(p.getX(i),min);
			}
			spread = max - min;
			if( largest_spread <= spread ){
				largest_spread = spread;
				largest_spread_dim = i;
			}
		}
		std::sort(points.begin(),points.end(),[&largest_spread_dim](Point a,Point b){return a.getX(largest_spread_dim)<b.getX(largest_spread_dim);});
		// 分割点
		int c0 = (points.size()-1)/2;
		int c1 = points.size()/2;
		Point split = (points[c0] + points[c1])/2.0f;

		std::vector<Point> c0v,c1v;
		for(int i = 0;i <=c0;i++)c0v.push_back(points[i]);
		for(int i = c1;i < points.size();i++)c1v.push_back(points[i]);
		child0 = (new BallTree(c0v,node_name+"-0"))->construct();
		child1 = (new BallTree(c1v,node_name+"-1"))->construct();
		return this;
	}
	BallTree* print(int indent = 0){
		for(int i = 0;i < indent;i++)printf("|");
		std::cout<<"-"<<node_name;
		if(child0 == nullptr && child1 == nullptr)printf(" (%d)",points[0].getId());
		std::cout<<std::endl;
		if(child0 != nullptr)child0->print(indent+1);
		if(child1 != nullptr)child1->print(indent+1);
		return this;
	}
	~BallTree(){
		SAFE_RELEASE(parent);
		SAFE_RELEASE(child0);
		SAFE_RELEASE(child1);
	}
};

int main(){
	std::vector<Point> points;
	std::random_device random;
	std::mt19937 mt(random());
	for(int i = 0;i < N;i++)points.push_back(*Point{i,D}.init(mt));
	std::cout<<points.size()<<std::endl;
	//for(auto p : points){p.print();}

	BallTree{points}.construct()->print();
}

