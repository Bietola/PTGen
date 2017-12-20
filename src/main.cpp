#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

#include "utils.h"

#include <SFML/Graphics.hpp>

using sf::Uint32;

//a Pythagoras Node, which is really just a square with a right-angled triangle sitting on top...
struct PNode {
   sf::Vector2f baseP1, baseP2, topP1, topP2, triangleP;

    auto calcVertexPositions() const {
        return std::vector<sf::Vector2f>{baseP1, baseP2, topP2, topP2, topP1, baseP1, topP1, topP2, triangleP};
        //!OLD return std::vector<sf::Vector2f>{baseP1, baseP2, topP1, topP2, triangleP};
    }
};

//the one and only Pythagoras Tree class
class PTree: public sf::Drawable, public sf::Transformable {
    public:
        PTree(float sideLength, float angle, Uint32 depth);

    private:
        std::vector<sf::Vertex> mVertices;

        static PNode generateNode(const sf::Vector2f& baseP1, const sf::Vector2f& baseP2, float angle);
        static std::vector<sf::Vector2f> generateTree(const sf::Vector2f& baseP1, const sf::Vector2f& baseP2, float angle, Uint32 depth);
    
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

//actually generates the tree class
//  NB: the functions below do the actual work
PTree::PTree(float sideLength, float angle, Uint32 depth) {
    //generate tree points
    auto treePoints = generateTree({0, 0}, {sideLength, 0}, angle, depth);

    //convert points into vertex array
    std::array<sf::Color, 3> colors = {
        sf::Color::White,
        sf::Color::Green,
        sf::Color::Yellow
    };
    mVertices.reserve(treePoints.size());
    std::transform(treePoints.begin(), treePoints.end(), std::back_inserter(mVertices),
        [&colors, cidx = 0, ccidx = 0] (const auto& ele) mutable {
            if(ccidx++ % 9 == 0) ++cidx;
            return sf::Vertex(ele, colors[cidx % 3]);
        }
    );
}

//generates the points of a single "node" given the construction parameters
//  NB: a "node" consists of a square with a right-angled triangle standing on top of it
PNode PTree::generateNode(const sf::Vector2f& baseP1, const sf::Vector2f& baseP2, float angle) {
    //get side length and versor
    auto sideLength = magnitude(baseP2 - baseP1);

    //get versors storing the directions of the base and height of the square
    auto baseV = normalize(baseP2 - baseP1);
    auto heightV = sf::Vector2f(baseV.y, -baseV.x);

    //calculate top base points
    auto topP1 = baseP1 + sideLength * heightV;
    auto topP2 = baseP2 + sideLength * heightV;

    //get versor poiting to tip of triangle (from the first top base point)
    auto angleV = rotateVector(baseV, -angle);

    //calculate tip of triangle by projecting the top base onto the triangle versor
    //  NB: this works because the triangle is right-angled
    auto triangleP = topP1 + projection(sideLength * baseV, angleV);

    //return obtained points as node
    return PNode{baseP1, baseP2, topP1, topP2, triangleP};
}

//generates a whole tree (returning its points in drawing order) given the parameters of the root node and
//the tree depth
std::vector<sf::Vector2f>
PTree::generateTree(const sf::Vector2f& baseP1, const sf::Vector2f& baseP2, float angle, Uint32 depth) {
    //base case - done generating the tree
    if(!depth)
        return {};

    //result of function
    std::vector<sf::Vector2f> treePoints;

    //generate the node and retrieve some needed data
    auto node = generateNode(baseP1, baseP2, angle);

    //recurse right
    //TODO auto rightTreePoints = generateTree(node.triangleP, node.topP2, angle, depth - 1);

    //recurse left
    auto leftTreePoints = generateTree(node.topP1, node.triangleP, angle, depth - 1); 

    //join outputs
    auto nodePoints = node.calcVertexPositions();
    treePoints.insert(treePoints.end(), nodePoints.begin(), nodePoints.end());
    //TODO treePoints.insert(treePoints.end(), rightTreePoints.begin(), rightTreePoints.end());
    treePoints.insert(treePoints.end(), leftTreePoints.begin(), leftTreePoints.end());

    //return
    return treePoints;
}

//draw the tree
void PTree::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    //set transform
    states.transform *= getTransform();

    //draw tree
    target.draw(&mVertices[0], mVertices.size(), sf::Triangles, states);
}


//The Main
int main() {
    //create application window
    sf::RenderWindow window(sf::VideoMode(1800, 1000), "PTGen");

    //create tree
    PTree ptree(200, 30, 10);
    ptree.move(900, 1000);

    //main loop
    sf::Event event;
    while(window.isOpen()) {
        //poll events
        while(window.pollEvent(event)) {
            //handle closing of window
            if(event.type == sf::Event::Closed ||
               event.type == sf::Event::KeyPressed)
                window.close();
        }

        //draw tree
        window.clear();
        window.draw(ptree);
        window.display();
    }
}
