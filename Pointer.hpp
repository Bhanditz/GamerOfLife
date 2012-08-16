#ifndef POINTER_HPP
#define POINTER_HPP

class Pointer {
 private:
    int x, y;
 public:
    Pointer(int x, int y) { set(x, y); }
    Pointer() { set(0, 0); }
    void move(int xp, int yp) {
        this->x += xp;
        this->y += yp;
    }
    void set(int x, int y){
        this->x = x;
        this->y = y;
    }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    int getX(){ return this->x; }
    int getY(){ return this->y; }
};

#endif // POINTER_HPP
