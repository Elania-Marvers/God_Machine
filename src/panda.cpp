#include "lib.h"



Panda::Panda(int width, int height, char *name)
{
  _win = bunny_start (width, height, false, name);
  _px  = bunny_new_pixelarray (width, height);
  clear(COLOR(255, 0, 0, 0));

  bunny_set_loop_main_function(__loop);
  bunny_set_key_response(event_handler);
  bunny_set_display_function(__display);
  bunny_loop (_win, 8, this);
}

Panda::~Panda()
{
  bunny_stop(_win);
}

void Panda::clear(unsigned int color)
{
  for (int i = 0; i < _px->clipable.buffer.width * _px->clipable.buffer.height; ++i)
    set_pixel(i, color);
}

/******************************COLOR******************************/
/******************************************************************/
/******************************COLOR******************************/


double Panda::color_ratio(int      value, 
			       int 	min,
			       int 	max)
{
  return ((double) value - (double) min) / ((double) max - (double) min);
}


unsigned char Panda::color_ratiod(unsigned char	from, 
				      unsigned char	to,
				      double		ratio)
{
  unsigned char result;
  result = (int) ((double) from * ((double) 1 - ratio) + (double) to * ratio);
  return result;
}


unsigned int Panda::mix_color (unsigned int ca, unsigned int cb, double percent)
{
  t_bunny_color a, b, c;
  a.full = ca;
  b.full = cb;
  
  c.argb[RED_CMP] = color_ratiod(a.argb[RED_CMP], b.argb[RED_CMP], percent);
  c.argb[GREEN_CMP] = color_ratiod(a.argb[GREEN_CMP], b.argb[GREEN_CMP], percent);
  c.argb[BLUE_CMP] = color_ratiod(a.argb[BLUE_CMP], b.argb[BLUE_CMP], percent);
  return c.full;
}

int Panda::getWidth() const
{
  return this->_px->clipable.buffer.width;
}

int Panda::getHeight() const
{
  return this->_px->clipable.buffer.height;
}


void Panda::set_xline(t_bunny_position *pos, unsigned int color)
{
  t_bunny_position draw, mi, ma;
  double c = 0.0;
  int vec;
  if (pos[0].x == std::max(pos[0].x, pos[1].x))
    {
      mi = pos[1];
      ma = pos[0];
    }
  else
    {
      mi = pos[0];
      ma = pos[1];
    }
  for (vec = ma.y - mi.y, draw.x = mi.x, draw.y = mi.y + vec * c;
       draw.x <= ma.x;
       c = (double) ((double) draw.x - (double) mi.x) / (double) ((double) ma.x - (double) mi.x),
	 draw.x += 1,
	 draw.y = mi.y + vec * c)
    set_pixel(draw, color);  
}


void Panda::set_yline(t_bunny_position *pos, unsigned int color)
{
  t_bunny_position draw, mi, ma;
  double c = 0.0;
  int vec;
  if (pos[0].y == std::max(pos[0].y, pos[1].y))
    {
      mi = pos[1];
      ma = pos[0];
    }
  else
    {
      mi = pos[0];
      ma = pos[1];
    }
  for (vec = ma.x - mi.x, draw.y = mi.y, draw.x = mi.x + vec * c;
       draw.y <= ma.y;
       c = (double) ((double) draw.y - (double) mi.y) / (double) ((double) ma.y - (double) mi.y),
	 draw.y += 1,
	 draw.x = mi.x + vec * c)
    set_pixel(draw, color);  
}



void Panda::set_eline(t_bunny_position *pos, unsigned int color)
{
  t_bunny_position draw, ma;
  if (pos[0].x == std::max(pos[0].x, pos[1].x))
    {
      ma = pos[0];
      draw = pos[1];
    }
  else
    {
      ma = pos[1];
      draw = pos[0];
    }
  
  for (; draw.x < ma.x; draw.x++)
    {
      set_pixel(draw, color);
      if (draw.y < ma.y)
	draw.y++;
      else
	draw.y--;
      
    }
}


void Panda::set_line(t_bunny_position *pos, unsigned int color)
{
  int x;
  int y;
  x = std::max(pos[0].x, pos[1].x) - std::min(pos[0].x, pos[1].x);
  y = std::max(pos[0].y, pos[1].y) - std::min(pos[0].y, pos[1].y);
  if (x > y)
      set_xline(pos, color);
  else if (x == y)
      set_eline(pos, color);
  else
      set_yline(pos, color);
}


void Panda::set_line(t_bunny_accurate_position pa, t_bunny_accurate_position pb, unsigned int color)
{
  t_bunny_position p[2];
  p[0] = {(int) pa.x, (int) pa.y};
  p[1] = {(int) pb.x, (int) pb.y};
  set_line(p, color);
}

void Panda::set_line(t_bunny_position pa, t_bunny_position pb, unsigned int color)
{
  t_bunny_position p[2];
  p[0] = pa;
  p[1] = pb;
  set_line(p, color);
}




void Panda::set_pixel(int x, int y, unsigned int color)
{
  t_bunny_position pos = {x, y};
  set_pixel(pos, color);
}


void Panda::set_pixel(t_bunny_position pos, unsigned int color)
{
  t_bunny_color old, _new;
  unsigned int *caze = (unsigned int *) _px->pixels;
  old.full = get_pixel(pos);
  _new.full = color;
  caze[pos.x + pos.y * _px->clipable.buffer.width] = mix_color(old.full, color, (double) _new.argb[ALPHA_CMP] / 255.0);
}

void Panda::set_pixel(t_bunny_accurate_position pos, unsigned int color)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  t_bunny_color old, _new;
  old.full = get_pixel(pos);
  _new.full = color;
  caze[(int) pos.x + (int) pos.y * _px->clipable.buffer.width] = mix_color(old.full, color, (double) _new.argb[ALPHA_CMP] / 255.0);
}

void Panda::set_pixel(int i, unsigned int color)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  t_bunny_color old, _new;
  old.full = caze[i];
  _new.full = color;
  caze[i] = mix_color(old.full, color, (double) _new.argb[ALPHA_CMP] / 255.0);
}

unsigned int Panda::get_pixel(t_bunny_position pos)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  return caze[pos.x + pos.y * _px->clipable.buffer.width];
}


unsigned int Panda::get_pixel(t_bunny_accurate_position pos)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  return caze[(int) pos.x + (int) pos.y * _px->clipable.buffer.width];
}



void Panda::set_square (t_bunny_position *pos, unsigned int color)
{
  t_bunny_position p[2];
  p[0] = pos[0];
  p[1] = {pos[1].x, pos[0].y};
  set_line(p, color);
  p[0] = pos[1];
  p[1] = {pos[0].x, pos[1].y};
  set_line(p, color);
  p[0] = pos[1];
  p[1] = {pos[1].x, pos[0].y};
  set_line(p, color);
  p[0] = pos[0];
  p[1] = {pos[0].x, pos[1].y};
  set_line(p, color);  
}


static t_bunny_accurate_position for_angle (t_bunny_position pos, int radius, double i)
{
  t_bunny_accurate_position draw;
  draw.x = pos.x + radius * cos (i);
  draw.y = pos.y - radius * sin (i);
  return draw;
}

void Panda::set_circle (t_bunny_position pos, int radius, unsigned int color)
{
  t_bunny_accurate_position draw = {(double) pos.x, (double) pos.y};
  for (double i = 0.0; i < M_PI * 2; i += ((M_PI * 2) / (360 * radius)))
    {
      draw = for_angle (pos, radius, i);
      set_pixel(draw, color);
    }
  
}

void Panda::god_machine(unsigned int color)
{
  unsigned int radius = (getWidth() < getHeight()) ? getWidth() / 2 : getHeight() / 2;
  t_bunny_position pos = {getWidth() / 2, getHeight() / 2};
  t_bunny_accurate_position pa;
  t_bunny_accurate_position pb;
  set_circle (pos, radius, color);
  set_circle (pos, radius * (1.0 / 2.0), color);
  set_circle (pos, radius * (1.0 / 4.0), color);
  double angle = (2 * M_PI) * (double) (rand()%36000 / 36000.0);
  double angle_ratio;

  pa= {(double) pos.x, (double) pos.y};
  pb= for_angle ((t_bunny_position) pos, radius * (1.0 / 4.0), angle);
  set_line (pa, pb, color);
  angle_ratio = angle + (2 * M_PI) * (rand()%25 / 100.0);
  pa = for_angle ((t_bunny_position) pos, radius * (1.0 / 2.0), angle_ratio);
  set_line (pa, pb, color);
  angle_ratio -= (2 * M_PI) * (rand()%25 / 100.0);
  pb = for_angle ((t_bunny_position) pos, radius, angle_ratio);
  set_line (pa, pb, color);
}
