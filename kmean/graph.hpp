//
// Created by Wan Luan Lee on 10/2/22.
//

#ifndef UNTITLED_GRAPH_HPP
#define UNTITLED_GRAPH_HPP

#endif //UNTITLED_GRAPH_HPP
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <memory>
#include <cmath>
#include <limits>
class Point;
class Graph;


class Point {

public:
    Point(const std::string& name, float x, float y);
    ~Point() = default;
    float get_x() const;
    float get_y() const;
    int get_cluster() const;
    std::string get_name() const;
    float calculate_distance(Point* b);
    void set_cluster(int cluster_id);
    bool visited = false;

private:
    float _x = 0;
    float _y = 0;
    int _cluster = -1;
    std::string _name = "";
};

Point::Point(const std::string& name, float x, float y) : _name{name}, _x{x}, _y{y} {
}

//Point::~Point(){}

float Point::get_x() const {
    return _x;
}

float Point::get_y() const {
    return _y;
}

void Point::set_cluster(int cluster_id) {
    _cluster = cluster_id;
}

int Point::get_cluster() const {
    return _cluster;
}

std::string Point::get_name() const{
    return _name;
}

float Point::calculate_distance(Point* b) {
    float dist = sqrt((b->get_x() - _x) * (b->get_x() - _x) + (b->get_y() - _y) * (b->get_y() - _y));
    return dist;
}

class Graph {
public:
    Graph(int k);
    std::vector<Point> get_all_points();
    //std::vector<Point*> get_center_points();
    bool create_graph(std::string input_file);
    void init_cluster();
    bool find_cluster();
    void update_mean();
    void output_graph (std::string out_file) const;

private:
    const int _k;
    std::vector<Point> _all_points = {};
    std::vector<std::shared_ptr<Point>> _center_points = {};
};

Graph::Graph(int k) : _k{k}{}

std::vector<Point> Graph::get_all_points() {
    return _all_points;
}

//std::vector<Point*> Graph::get_center_points() {
    //std::vector<Point*> centers = {}
    //for(auto i : _center_points) {
       //centers.push(i.get());
    //}
    //return centers;
//}

bool Graph::create_graph(std::string input_file) {
    std::ifstream inputFileStream(input_file);
    //if file doesn't open then return false
    if (!inputFileStream.is_open()) {
        return false;
    }
    std::string line;
    while(!inputFileStream.eof()) {
        std::getline(inputFileStream, line);
        std::stringstream ss(line);
        std::string token;
        float x = -1.0;
        float y = -1.0;
        std::string name = "";
        while (ss >> token) {
            //std::cout << "token " <<token;
            if(token == ";") {}
            else {
                if(token[0] == 'p') {
                    name = token;
                }
                else if(x == -1) {
                    x = std::stof(token);
                }
                else {
                    y = std::stof(token);
                }
            }
            //Point new_point = Point(name,x,y);
        }
        _all_points.push_back(Point(name,x,y));
    }
    return true;
}

void Graph::init_cluster() {
    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_int_distribution<int> distribution(0,_all_points.size() - 1);
    for(int i = 0; i < _k; ++i) {
        Point &center_point = _all_points.at(distribution(mt));
        std::shared_ptr<Point> mean_point (new Point ("c",center_point.get_x(), center_point.get_y()));
        _center_points.push_back(mean_point);
        _center_points.back()->set_cluster(i);
        _center_points.back()->visited = true;
    }
}

bool Graph::find_cluster() {
    bool if_coverage = true;
    for (auto &&i: _all_points) {
        float min_dist = std::numeric_limits<double>::max();
        if (!i.visited) {
            if(i.get_cluster() != -1) {
                min_dist = i.calculate_distance(_center_points.at(i.get_cluster()).get());
            }
            for (int j = 0; j < _center_points.size(); ++j) {
                if(j != i.get_cluster()) {
                    float dist = i.calculate_distance(_center_points.at(j).get());
                    if (min_dist > dist) {
                        int old_cluster = i.get_cluster();
                        if(old_cluster != _center_points.at(j)->get_cluster()) {
                            i.set_cluster(_center_points.at(j)->get_cluster());
                           // std::cerr << i.get_name() << " old cluster " << old_cluster << "new cluster " <<  i.get_cluster() << "\n";
                            if_coverage = false;
                            min_dist = dist;

                        }
                    }
                }

            }
            i.visited = true;
        }
    }

    return if_coverage;
}

void Graph::update_mean() {
    std::vector<int> num_points = {};
    std::vector<float> sum_x = {};
    std::vector<float> sum_y = {};

    //init vectors
    for(int i = 0; i < _k; ++i) {
        num_points.push_back(0);
        sum_x.push_back(0);
        sum_y.push_back(0);
    }

    for(auto&& i : _all_points) {
        num_points.at(i.get_cluster()) += 1;
        sum_x.at(i.get_cluster()) += i.get_x();
        sum_y.at(i.get_cluster()) += i.get_y();
        i.visited = false;
    }

    //calculate center point
    _center_points.clear();
    for(int i = 0; i < _k; ++i) {
        float new_x = sum_x.at(i) / num_points.at(i);
        float new_y = sum_y.at(i) / num_points.at(i);
        std::shared_ptr<Point> mean_point (new Point ("c",new_x, new_y));
        _center_points.push_back(mean_point);
        _center_points.back()->set_cluster(i);
    }
}

void Graph::output_graph (std::string out_file) const {
    std::ofstream o_file;
    o_file.open(out_file);
    o_file << "p,x,y,c \n";

    for(auto&& i : _all_points){
        o_file << i.get_name() <<"," << i.get_x() << "," << i.get_y() << "," << i.get_cluster()<< "\n";
    }
}
