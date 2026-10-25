
/*NAME:
   Afroze Shaikh - 23281
   Shravani Sonawane - 23282
   Rohini Ahire - 23272 
   Shravani Deshmukh */

//METRO ROUTE FINDER SYSTEM 
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <cctype>
using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int to;
    int time;
    string line;
};

class Metro {
private:
    vector<string> stations;
    vector<vector<Edge>> graph;

    string lower(string s) {
        transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return tolower(c); });
        return s;
    }

    int findStation(string name) {
        name = lower(name);

        for (int i = 0; i < (int)stations.size(); i++) {
            if (lower(stations[i]) == name)
                return i;
        }
        return -1;
    }

    void addStation(string name) {
        stations.push_back(name);
        graph.push_back({});
    }

    void addConnection(int a, int b, int time, string line) {
        graph[a].push_back({b, time, line});
        graph[b].push_back({a, time, line});
    }

    void displayRoute(vector<int> route,
                      vector<string> usedLines,
                      int totalTime) {
        if (route.empty()) {
            cout << "\nNo route found!\n";
            return;
        }

        stack<int> path;
        for (int station : route)
            path.push(station);

        cout << "\n========== YOUR METRO ROUTE ==========\n";
        cout << "Start: " << stations[route.front()] << "\n";
        cout << "Destination: " << stations[route.back()] << "\n\n";

        cout << "Route:\n";
        while (!path.empty()) {
            cout << stations[path.top()];
            path.pop();

            if (!path.empty())
                cout << " -> ";
        }

        cout << "\n\nJourney details:\n";

        int interchanges = 0;
        string previousLine = "";

        for (int i = 0; i < (int)usedLines.size(); i++) {
            if (i > 0 && usedLines[i] != previousLine)
                interchanges++;

            cout << stations[route[i]] << " -> "
                 << stations[route[i + 1]]
                 << " | Line: " << usedLines[i] << "\n";

            previousLine = usedLines[i];
        }

        cout << "\nTotal travel time: " << totalTime
             << " minutes (estimated)\n";
        cout << "Number of stops: " << route.size() - 1 << "\n";
        cout << "Interchanges: " << interchanges << "\n";
        cout << "======================================\n";
    }

    void reconstruct(int source, int dest,
                     vector<int>& parent,
                     vector<string>& parentLine,
                     int totalTime) {
        if (source != dest && parent[dest] == -1) {
            cout << "\nNo route exists between these stations.\n";
            return;
        }

        vector<int> route;
        vector<string> lines;

        int current = dest;
        while (current != source) {
            route.push_back(current);
            lines.push_back(parentLine[current]);
            current = parent[current];
        }
        route.push_back(source);

        reverse(route.begin(), route.end());
        reverse(lines.begin(), lines.end());

        displayRoute(route, lines, totalTime);
    }

public:
    Metro() {
        // Sample Pune Metro network for demonstration.
        // Verify station connectivity and timings before real use.

        string names[] = {
            "PCMC", "Sant Tukaram Nagar", "Bhosari",
            "Kasarwadi", "Phugewadi", "Dapodi",
            "Bopodi", "Khadki", "Range Hill",
            "Shivaji Nagar", "Civil Court", "Budhwar Peth",
            "Mandai", "Swargate", "PMC",
            "Mangalwar Peth", "Pune Railway Station",
            "Ruby Hall Clinic", "Bund Garden", "Yerawada",
            "Kalyani Nagar", "Ramwadi", "Vanaz",
            "Anand Nagar", "Ideal Colony", "Nal Stop",
            "Garware College", "Deccan Gymkhana",
            "Chhatrapati Sambhaji Udyan"
        };

        int count = sizeof(names) / sizeof(names[0]);

        for (int i = 0; i < count; i++)
            addStation(names[i]);

        // Purple Line: sample connections
        for (int i = 0; i < 10; i++)
            addConnection(i, i + 1, 3, "Purple");

        addConnection(10, 11, 3, "Purple");
        addConnection(11, 12, 3, "Purple");
        addConnection(12, 13, 4, "Purple");

        // Aqua Line: sample connections
        addConnection(10, 14, 3, "Aqua");
        addConnection(14, 15, 3, "Aqua");
        addConnection(15, 16, 3, "Aqua");
        addConnection(16, 17, 3, "Aqua");
        addConnection(17, 18, 3, "Aqua");
        addConnection(18, 19, 3, "Aqua");
        addConnection(19, 20, 3, "Aqua");
        addConnection(20, 21, 3, "Aqua");

        // Other Aqua Line stations, towards Vanaz
        addConnection(22, 23, 3, "Aqua");
        addConnection(23, 24, 3, "Aqua");
        addConnection(24, 25, 3, "Aqua");
        addConnection(25, 26, 3, "Aqua");
        addConnection(26, 27, 3, "Aqua");
        addConnection(27, 28, 3, "Aqua");
        addConnection(28, 14, 3, "Aqua");
    }

    void showStations() {
        cout << "\n========== AVAILABLE STATIONS ==========\n";
        for (int i = 0; i < (int)stations.size(); i++)
            cout << i + 1 << ". " << stations[i] << "\n";
    }

    void searchStation() {
        string key;

        cout << "\nEnter station name to search: ";
        getline(cin, key);

        bool found = false;
        cout << "\nSearch results:\n";

        for (int i = 0; i < (int)stations.size(); i++) {
            if (lower(stations[i]).find(lower(key))
                != string::npos) {
                cout << "- " << stations[i] << "\n";
                found = true;
            }
        }

        if (!found)
            cout << "No matching station found.\n";
    }

    void shortestRoute() {
        string start, destination;

        cout << "\nEnter source station: ";
        getline(cin, start);

        cout << "Enter destination station: ";
        getline(cin, destination);

        int source = findStation(start);
        int dest = findStation(destination);

        if (source == -1 || dest == -1) {
            cout << "\nInvalid station name. "
                 << "Use Search or View Stations.\n";
            return;
        }

        vector<int> dist(stations.size(), INF);
        vector<int> parent(stations.size(), -1);
        vector<string> parentLine(stations.size(), "");

        // Min-priority queue for Dijkstra
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d != dist[u])
                continue;

            for (const Edge& e : graph[u]) {
                if (dist[u] + e.time < dist[e.to]) {
                    dist[e.to] = dist[u] + e.time;
                    parent[e.to] = u;
                    parentLine[e.to] = e.line;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        if (dist[dest] == INF) {
            cout << "\nNo route available.\n";
            return;
        }

        reconstruct(source, dest, parent,
                    parentLine, dist[dest]);
    }

    void fewestStopsRoute() {
        string start, destination;

        cout << "\nEnter source station: ";
        getline(cin, start);

        cout << "Enter destination station: ";
        getline(cin, destination);

        int source = findStation(start);
        int dest = findStation(destination);

        if (source == -1 || dest == -1) {
            cout << "\nInvalid station name.\n";
            return;
        }

        vector<int> parent(stations.size(), -1);
        vector<string> parentLine(stations.size(), "");
        vector<int> dist(stations.size(), -1);
        queue<int> q;

        dist[source] = 0;
        q.push(source);

        // BFS finds the route with the fewest edges.
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const Edge& e : graph[u]) {
                if (dist[e.to] == -1) {
                    dist[e.to] = dist[u] + 1;
                    parent[e.to] = u;
                    parentLine[e.to] = e.line;
                    q.push(e.to);
                }
            }
        }

        if (dist[dest] == -1) {
            cout << "\nNo route available.\n";
            return;
        }

        // Calculate the estimated time for the BFS route.
        int totalTime = 0;
        int current = dest;

        while (current != source) {
            int previous = parent[current];

            for (const Edge& e : graph[previous]) {
                if (e.to == current) {
                    totalTime += e.time;
                    break;
                }
            }

            current = previous;
        }

        reconstruct(source, dest, parent,
                    parentLine, totalTime);

        cout << "Route selected by BFS: fewest station connections.\n";
    }

    void run() {
        int choice;

        do {
            cout << "\n\n====================================\n";
            cout << "       METRO ROUTE FINDER\n";
            cout << "====================================\n";
            cout << "1. Find shortest travel-time route\n";
            cout << "2. Find route with fewest stops\n";
            cout << "3. Search for a station\n";
            cout << "4. View all stations\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    shortestRoute();
                    break;
                case 2:
                    fewestStopsRoute();
                    break;
                case 3:
                    searchStation();
                    break;
                case 4:
                    showStations();
                    break;
                case 5:
                    cout << "\nThank you for using Metro Route Finder!\n";
                    break;
                default:
                    cout << "\nInvalid choice. Try again.\n";
            }

        } while (choice != 5);
    }
};

int main() {
    Metro metro;
    metro.run();
    return 0;
}
