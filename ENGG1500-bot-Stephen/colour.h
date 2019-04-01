#ifndef COLOUR_H
#define COLOUR_H

#define RED 13
#define GREEN 11
#define BLUE 12

typedef struct colour{
  int red;
  int green;
  int blue;
};

colour new_colour(int r, int g, int b)
{
  colour* c = malloc(sizeof(colour));
  c->red = r;
  c->green = g;
  c->blue = b;
  return *c;
}

colour blue = new_colour(0, 0, 10);
colour red = new_colour(10, 0, 0);
colour green = new_colour(0, 10, 0);
colour yellow = new_colour(7, 5, 0);
colour aqua = new_colour(0, 10, 1);
colour pink = new_colour(10, 1, 1);
colour purple = new_colour(5, 0, 5);

void colourChange(colour& b)
{
  digitalWrite(RED, HIGH);
  delay(b.red);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  delay(b.green);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
  delay(b.blue);
  digitalWrite(BLUE, LOW);
  
}

#endif
