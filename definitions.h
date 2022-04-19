enum State {
    DEAD=0, ALIVE
};

typedef struct Cell {
    State state;
    int x, y;
} Cell;