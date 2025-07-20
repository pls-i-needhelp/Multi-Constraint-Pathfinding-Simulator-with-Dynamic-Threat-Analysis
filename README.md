# Tactical A* Pathfinding Engine with Real-time Danger Assessment

A sophisticated C++ implementation of tactical pathfinding inspired by gameplay mechanics. This project demonstrates advanced Data Structures & Algorithms concepts through intelligent route planning that balances safety, efficiency, and tactical positioning.

## Overview

This pathfinding system simulates tactical movement decisions in combat scenarios where agents must navigate from spawn points to objectives while avoiding enemy fire, utilizing cover positions, and minimizing exposure to danger zones. The implementation showcases multiple DSA concepts working together to solve complex multi-constraint optimization problems.

## Key Features

### Core Algorithm Implementation
- **Advanced A* Pathfinding**: Modified cost function considering tactical elements
- **Dynamic Danger Assessment**: Real-time calculation of threat zones from multiple sources
- **Multi-Constraint Optimization**: Balances distance, safety, and tactical advantage
- **Obstacle Avoidance**: Intelligent navigation around walls and barriers

### Tactical Elements
- **Bomb Blast Zones**: Circular danger areas with distance-based intensity falloff
- **Cover Positions**: Tactical advantages that reduce movement costs
- **Strategic Obstacles**: Walls and barriers that create tactical chokepoints
- **Risk Assessment**: Real-time evaluation of exposure levels

### Visualization & Analysis
- **ASCII Map Visualization**: Real-time display of tactical scenarios
- **Path Animation**: Visual representation of chosen routes
- **Performance Metrics**: Detailed statistics on path efficiency and safety
- **Interactive Map Editor**: Easy modification of tactical scenarios

## Technical Implementation

### Data Structures Used
- **2D Vector Grid**: Efficient map representation with O(1) cell access
- **Priority Queue**: Min-heap for A* node exploration optimization
- **Hash Maps**: Fast lookup for visited nodes and parent tracking
- **Adjacency Lists**: Graph representation for movement possibilities

### Algorithms Implemented
- **A* Search Algorithm**: Optimal pathfinding with tactical heuristics
- **Dijkstra-based Cost Calculation**: Weighted edge traversal
- **Manhattan Distance Heuristic**: Efficient goal-distance estimation
- **Circular Falloff Functions**: Realistic danger zone modeling

### Complexity Analysis
- **Time Complexity**: O(b^d) where b = branching factor, d = depth
- **Space Complexity**: O(n) where n = number of map cells
- **Optimization**: Early termination and pruning for real-time performance

## Quick Start

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Standard C++ library support

### Compilation
```bash
# Basic compilation
g++ -std=c++17 cs2_tactical_path.cpp -o pathfind

# Optimized release build
g++ -std=c++17 -O3 -DNDEBUG cs2_tactical_path.cpp -o pathfind

# Debug build with extensive checking
g++ -std=c++17 -Wall -Wextra -g cs2_tactical_path.cpp -o pathfind_debug
```

### Execution
```bash
./pathfind              # Linux/macOS
pathfind.exe            # Windows
```

## Example Output

```
=== MAP BEFORE SEARCH ===
. . . . . . . . . . . . . . .
. . . . . . o o o . . . . G .
. . . . . . o ! o . . . . . .
. . . . . . o o o . . . # # .
. . . . . . X o o . . . . . .
. . . # # . X ! B . . . . . .
. . . . . . X o o . . . . . .
. . . . . . X . . . . . . . .
. . . . . . X . . . . . . . .
. S . . . . . . . . . . . . .

=== MAP WITH PATH ===
. . . . . . . . . . . . . . .
. . . . . . o o o . . . . G .
. . . . . . o ! o . . . . * .
. . . . . . o o o . . . # * .
. . . . . . X o o . . . * . .
. . . # # . X ! B . . . * . .
. . . . . . X o o . . * . . .
. . . . . . X . . . * . . . .
. . * * * * X . . * . . . . .
. S * . . . . . * . . . . . .

Path length : 21
Danger sum  : 1.43
```

### Legend
- `S` - Start position
- `G` - Goal position  
- `*` - Optimal path
- `#` - Cover positions
- `X` - Obstacles/walls
- `B` - Bomb locations
- `!` - High danger zones
- `o` - Medium danger zones
- `.` - Safe areas

## Customization Options

### Map Configuration
```cpp
// Adjust grid dimensions
Map map(20, 15);  // width x height

// Add tactical elements
map.addCover(x, y, 0.8);        // Cover value 0-1
map.addObstacle(x, y);          // Impassable walls
map.addBomb(x, y, radius);      // Danger source
```

### Cost Function Tuning
```cpp
// Modify tactical priorities
double moveCost = 1.0                    // Base movement
                + c.danger * 5.0         // Danger penalty
                - c.cover * 0.4;         // Cover bonus

// Safety-focused: increase danger penalty
double moveCost = 1.0 + c.danger * 10.0 - c.cover * 0.4;

// Speed-focused: decrease danger penalty  
double moveCost = 1.0 + c.danger * 2.0 - c.cover * 0.4;
```

## Architecture

```
├── Cell Structure
│   ├── Terrain type (OPEN/COVER/OBSTACLE/BOMB)
│   ├── Danger level (0.0 - 1.0)
│   └── Cover value (0.0 - 1.0)
├── Map Class
│   ├── Grid management
│   ├── Tactical element placement
│   └── Visualization system
└── A* Algorithm
    ├── Priority queue management
    ├── Cost function evaluation
    └── Path reconstruction
```

## Performance Metrics

The system tracks multiple performance indicators:

- **Path Length**: Total number of movement steps
- **Danger Sum**: Cumulative risk exposure along route
- **Algorithm Efficiency**: Nodes explored vs optimal path length
- **Memory Usage**: Grid size vs path storage requirements

## Tactical Scenarios

### Scenario Examples
1. **Direct Assault**: Short path through danger zones
2. **Flanking Maneuver**: Longer but safer perimeter routes  
3. **Cover-to-Cover**: Tactical movement using protection points
4. **Chokepoint Navigation**: Strategic passage through narrow corridors

## Advanced Extensions

### Potential Enhancements
- **Diagonal Movement**: 8-direction pathfinding support
- **Dynamic Obstacles**: Real-time map modifications
- **Multi-Agent Coordination**: Team-based pathfinding
- **Machine Learning**: Adaptive cost function optimization
- **Visualization Improvements**: Color-coded terminal output or GUI

## Testing & Validation

### Unit Tests
- Path optimality verification
- Danger calculation accuracy
- Obstacle collision detection
- Edge case handling (no path scenarios)

### Performance Benchmarks
- Algorithm execution time analysis
- Memory usage profiling
- Scalability testing with larger maps
- Cost function sensitivity analysis

## Algorithm Details

### Cost Function Design
The tactical cost function implements a multi-objective optimization:

```
Total_Cost = Σ(Movement_Cost) for each step in path

Where Movement_Cost = Base_Cost + Danger_Penalty - Cover_Bonus
```

This creates emergent tactical behavior where:
- Agents avoid high-danger areas when alternatives exist
- Cover positions become attractive waypoints
- Risk vs efficiency trade-offs are automatically balanced

### Heuristic Function
Uses Manhattan distance for admissible heuristic ensuring optimality:

```cpp
h(n) = |n.x - goal.x| + |n.y - goal.y|
```

## Learning Outcomes

This project demonstrates mastery of:

**Data Structures**: 2D arrays, priority queues, hash maps, graph representation  
**Algorithms**: A* search, shortest path algorithms, heuristic optimization  
**System Design**: Modular architecture, extensible frameworks  
**Problem Solving**: Multi-constraint optimization, real-time decision making  
**Software Engineering**: Clean code practices, performance optimization

## Contributing

Contributions are welcome! Areas for improvement:
- Performance optimizations
- Additional tactical elements
- Enhanced visualization
- Algorithm variants (Dijkstra, JPS, etc.)
- Unit test coverage expansion

## License

This project is open source and available under the [MIT License](LICENSE).

## Future Roadmap

- [ ] Web-based interactive visualizer
- [ ] Multi-threaded pathfinding for large maps
- [ ] Real-time replanning for dynamic scenarios
- [ ] Integration with game engines (Unity/Unreal)
- [ ] Machine learning-based cost function optimization

---
 
