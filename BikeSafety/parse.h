#include <expat.h>
#include <boost/unordered_map.hpp>
#include <vector>
#include <fstream>
#include <sstream>


typedef uint64_t node_t;
const int unknown = -1;
const int foot_forbiden = 0;
const int foot_allowed = 1;

const int car_forbiden = 0;
const int car_residential = 1;
const int car_tertiary = 2;
const int car_secondary = 3;
const int car_primary = 4;
const int car_trunk = 5;
const int car_motorway = 6;

const int bike_forbiden = 0;
const int bike_opposite_lane = 1;
const int bike_allowed = 2;
const int bike_lane = 3;
const int bike_busway = 4;
const int bike_track = 5;

class Edge_property
{
    public:
    int car_direct;
    int car_reverse;
    int bike_direct;
    int bike_reverse;
    int foot;

    Edge_property();

    // Can at least one mean use that edge
    bool accessible();
    bool direct_accessible();
    bool reverse_accessible();

    // Update the properties given new information
    bool update(const std::string & tag, const std::string & val);

    // Infer unknown data
    void normalize();

    void reset();
};

struct Node
{
    node_t id;
    double lon;
    double lat;
    
    // By how many ways has this node been used
    // This allows to detect a street intersection
    char uses;

    // Is this node valid:
    // In some exports, a way references a node that isn’t in the data
    // This causes bad coordinates
    bool valid;

    Node() : uses(0), valid(false) {};

    Node(double _lon, double _lat, node_t _id) :
        id(_id), lon(_lon), lat(_lat), uses(0), valid(true)
    {};
};

struct Edge
{ 
    node_t edge_id;
    node_t source;
    node_t target;
    float length;
    char car;
    char car_d;
    char bike;
    char bike_d;
    char foot;
    std::string geom;
    Edge() {}
    Edge(node_t e, node_t s, node_t t, float l, char c, char cd, char b, char bd, char f, const std::string & str) :
        edge_id(e), source(s), target(t), length(l),
        car(c), car_d(cd), bike(b), bike_d(bd), foot(f),
        geom(str)
    {}
};


typedef boost::unordered_map<uint64_t, Node> NodeMapType;

struct Parser
{
    Node * source;
    Node * prev;
    NodeMapType nodes;
    node_t ways_count;
    int edge_length;
    node_t ways_progress;
    Edge_property ep;
    double length;
    XML_Parser parser;
    std::ofstream temp_edges;
    std::vector<node_t> way_nodes;
    node_t current_way;

    Parser();
    void read(char *, int, bool);
    std::vector<Node> get_nodes() const;
    std::vector<Edge> get_edges() const;
    unsigned long get_osm_nodes() const;
    unsigned long long get_osm_ways() const;

};
