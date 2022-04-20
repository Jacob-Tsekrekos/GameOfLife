typedef enum State {
    DEAD=0, ALIVE
} State;

typedef struct Cell {
    State state;
    int x, y;
} Cell;