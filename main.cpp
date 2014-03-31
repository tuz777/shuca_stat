#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Check_Button.H>
#include <Fl/gl.H>

#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

class vec{
	public:
		double x, y;
};

class genotype{
    public:
        double eye_r, type, max_age, v_mod, intellect, agra, mult, a, b, yleg, xleg;
};

class mywin : public Fl_Gl_Window{
    public:    
        mywin(int, int, int, int, const char*);
        
        //void draw_gen(genotype*);
        void draw_line(vec, vec, double, double, double);
        void draw_gen(genotype);
        
        double czoom, fzoom;
        vector<double> fbcon, cbcon;
        vector<int> csize;
        vector<genotype> gcon;
        int gdraw;

        void draw();
};

mywin::mywin(int x, int y, int w, int h, const char *label) : Fl_Gl_Window(x, y, w, h, label)
{
    fzoom = 0;
    czoom = 0;
    gdraw = 0;
    
    FILE* f = fopen("output.txt", "r");
    for(int i = 0; !feof(f); i++)
    {
        double fbiomass, cbiomass;
        int csz;
        genotype to_add;
        fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d", &fbiomass, &cbiomass, &(to_add.a), &(to_add.b), &(to_add.xleg), &(to_add.yleg), &(to_add.eye_r ), &(to_add.max_age), &(to_add.intellect), &(to_add.v_mod), &(to_add.agra), &(to_add.mult), &(to_add.type), &csz);
        fbcon.push_back(fbiomass);
        cbcon.push_back(cbiomass);
        gcon.push_back(to_add);
        csize.push_back(csz);
    }
}
    

void mywin::draw_line(vec a, vec b, double R, double G, double B)
{
    glLoadIdentity();
	glColor3f(R, G, B);
	glBegin(GL_LINES);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
}
void mywin::draw_gen(genotype c)
{
	glLoadIdentity();
	
	glScalef(czoom, czoom, czoom);
	
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0 - c.type, 0.0, c.type);
	glVertex2f(0.0, 0.0);
	for(int i = 0; i <= 20; i++)
	{
		float an = float(i)/10 * M_PI;
		glVertex2f(cos(an)*c.a, sin(an)*c.b);
	}
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(-(c.a + c.xleg), 0.0); glVertex2f(c.a + c.xleg, 0.0);
	glVertex2f(0.0, -(c.b + c.yleg)); glVertex2f(0.0, c.b + c.yleg);
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	//glVertex2f(0.0, 0.0);
	glColor3f(0.0, 0.3, 1.0);
	for(int i = 0; i <= 50; i++)
	{
		float an = float(i)/25 * M_PI;
		glVertex2f(cos(an)*c.eye_r, sin(an)*c.eye_r);
	}
	glEnd();
	
	glLoadIdentity();
	
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-1, 0.9); glVertex2f(c.v_mod*czoom*10 - 1, 0.9);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(-1, 0.8); glVertex2f(c.intellect*czoom*10 - 1, 0.8);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(-1, 0.7); glVertex2f(c.max_age*czoom/10 - 1, 0.7);
	glColor3f(0.9, 0.75, 0.0);
	glVertex2f(-1, 0.6); glVertex2f(c.agra*czoom*10 - 1, 0.6);
	glVertex2f(-1, 0.5); glVertex2f(c.mult*czoom*10 - 1, 0.5);
    //glVertex2f(0.0, -(c.b + c.yleg)); glVertex2f(0.0, c.b + c.yleg);
	glEnd();
} 

void mywin::draw()
{
    glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    glLineWidth(5.0);
            
    glMatrixMode(GL_MODELVIEW);
    
    for(int i = 1; i < fbcon.size(); i++)
    {
        vec a, b; a.x = -1 + (2.0/fbcon.size())*i; a.y = -1; b.x = a.x; b.y = fbcon[i]*fzoom - 1;
        draw_line(a, b, 0.0, 0.5, 0.5);
        a.x = -1 + (2.0/cbcon.size())*i; a.y = fbcon[i]*fzoom - 1; b.x = a.x; b.y = cbcon[i]*fzoom + a.y;
        draw_line(a, b, 0.6, 0.8, 0.0);
        a.x = -1 + (2.0/cbcon.size())*(i - 1); a.y = csize[i-1]*fzoom*10000 - 1; b.x = -1 + (2.0/cbcon.size())*i; b.y = csize[i]*fzoom*10000 - 1;
        draw_line(a, b, 0.0, 0.0, 0.0);
    }
    draw_gen(gcon[gdraw]);
} 
            

/*void mywin::draw_gen(genotype* c)
{
	glLoadIdentity();
	
	glScalef(k, k, k);
	
	glTranslatef(c->loc.x + center.x, c->loc.y + center.y, 0.0);
	glRotatef(c->angle, 0.0, 0.0, 1.0);
	
	if(c->gen.type == 0)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 0.0, 1.0);
	
	
    glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0, 0.0);
	for(int i = 0; i <= 20; i++)
	{
		float an = float(i)/10 * M_PI;
		glVertex2f(cos(an)*c->gen.a, sin(an)*c->gen.b);
	}
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(-(c->gen.a + c->gen.xleg), 0.0); glVertex2f(c->gen.a + c->gen.xleg, 0.0);
	glVertex2f(0.0, -(c->gen.b + c->gen.yleg)); glVertex2f(0.0, c->gen.b + c->gen.yleg);
	glEnd();
}*/  

void fz(Fl_Widget* w, void* p)
{
    ((mywin*)p)->fzoom = ((Fl_Hor_Slider*)w)->value();
    ((mywin*)p)->redraw();
}
void cz(Fl_Widget* w, void* p)
{
    ((mywin*)p)->czoom = ((Fl_Hor_Slider*)w)->value();
    ((mywin*)p)->redraw();
}
void p(Fl_Widget* w, void* p)
{
    ((mywin*)p)->gdraw = int(((Fl_Hor_Slider*)w)->value());
    ((mywin*)p)->redraw();
}

int main()
{
    Fl_Window *window;

    window = new Fl_Window (Fl::h() - 200, Fl::h() - 70);
    mywin* win = new mywin(0, 0, Fl::h() - 200, Fl::h() - 200, "test");
    Fl_Hor_Slider* fzoom = new Fl_Hor_Slider(0, Fl::h() - 150, Fl::h() - 200, 20, "fzoom");
    fzoom->bounds(0, 0.000001);
    fzoom->callback(fz, win);
    Fl_Hor_Slider* czoom = new Fl_Hor_Slider(0, Fl::h() - 190, Fl::h() - 200, 20, "czoom");
    czoom->bounds(0, 0.05);
    czoom->callback(cz, win);
    Fl_Hor_Slider* ph = new Fl_Hor_Slider(0, Fl::h() - 110, Fl::h() - 200, 20, "ph");
    ph->bounds(0, win->gcon.size());
    ph->callback(p, win);
    
    /*Fl_Text_Display d(0, 0, 50, 65, "");
    Fl_Text_Buffer* buf = new Fl_Text_Buffer(100);
    d.buffer(buf);
    buf->insert(1, "eyer\nmaxa\ninte\nvmod");*/


    //window->fullscreen();
    window->show();
    win->draw();

    return(Fl::run());
}
