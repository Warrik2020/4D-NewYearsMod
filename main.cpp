//#define DEBUG_CONSOLE // Uncomment this if you want a debug console to start. You can use the Console class to print. You can use Console::inStrings to get input.

#include <4dm.h>

using namespace fdm;

// Initialize the DLLMain
initDLL

void viewportCallback(void* user, const glm::ivec4& pos, const glm::ivec2& scroll)
{
	GLFWwindow* window = (GLFWwindow*)user;

	// update the render viewport
	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	glViewport(pos.x, wHeight - pos.y - pos.w, pos.z, pos.w);

	// create a 2D projection matrix from the specified dimensions and scroll position
	glm::mat4 projection2D = glm::ortho(0.0f, (float)pos.z, (float)pos.w, 0.0f, -1.0f, 1.0f);
	projection2D = glm::translate(projection2D, { scroll.x, scroll.y, 0 });

	// update all 2D shaders
	const Shader* textShader = ShaderManager::get("textShader");
	textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textShader->id(), "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* tex2DShader = ShaderManager::get("tex2DShader");
	tex2DShader->use();
	glUniformMatrix4fv(glGetUniformLocation(tex2DShader->id(), "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* quadShader = ShaderManager::get("quadShader");
	quadShader->use();
	glUniformMatrix4fv(glGetUniformLocation(quadShader->id(), "P"), 1, GL_FALSE, &projection2D[0][0]);
}

TexRenderer texRenderer{};
TexRenderer texRenderer2{};
TexRenderer texRenderer3{};
QuadRenderer qr{};
FontRenderer font{};

// Gui objects here
gui::Text newyeartext{};
gui::Image confetti{};
gui::Image party{};
gui::Image look{};

$hook(void, StateTitleScreen, init, StateManager& s)
{
	texRenderer.texture = ResourceManager::get("confetti.png", true);
	texRenderer.shader = ShaderManager::get("tex2DShader");
	texRenderer.init();

	texRenderer2.texture = ResourceManager::get("party.png", true);
	texRenderer2.shader = ShaderManager::get("tex2DShader");
	texRenderer2.init();

	texRenderer3.texture = ResourceManager::get("LOOK.png", true);
	texRenderer3.shader = ShaderManager::get("tex2DShader");
	texRenderer3.init();

	newyeartext.alignX(gui::ALIGN_CENTER_X);
	newyeartext.alignY(gui::ALIGN_TOP);
	newyeartext.text = "Happy new year!!";
	newyeartext.size = 8;
	newyeartext.color = glm::vec4(1, 1, 0, 1);

	confetti.tr = &texRenderer;
	confetti.alignX(gui::ALIGN_CENTER_X);
	confetti.alignY(gui::ALIGN_BOTTOM);
	confetti.tr->setColor(1, 1, 1, 1.f);
	confetti.width = 800;
	confetti.height = 400;

	party.tr = &texRenderer2;
	party.width = confetti.tr->texture->width;
	party.height = confetti.tr->texture->height;
	party.alignX(gui::ALIGN_RIGHT);
	party.alignY(gui::ALIGN_BOTTOM);
	party.tr->setColor(1, 1, 1, 1.f);

	look.tr = &texRenderer3;
	look.width = look.tr->texture->width + 40;
	look.height = look.tr->texture->height + 40;
	look.alignX(gui::ALIGN_LEFT);
	look.alignY(gui::ALIGN_BOTTOM);
	look.tr->setColor(1, 1, 1, 1.f);
	

	self->instanceObj->ui.addElement(&newyeartext);
	self->instanceObj->ui.addElement(&confetti);
	self->instanceObj->ui.addElement(&party);
	self->instanceObj->ui.addElement(&look);

	original(self, s);
}

$hook(void, StateGame, init, StateManager& s)
{
	// Your code that runs at first frame here (it calls when you load into the world)

	original(self, s);
}

$hook(void, Player, update, World* world, double dt, EntityPlayer* entityPlayer)
{
	// Your code that runs every frame here (it only calls when you play in world, because its Player's function)

	original(self, world, dt, entityPlayer);
}

$hook(bool, Player, keyInput, GLFWwindow* window, World* world, int key, int scancode, int action, int mods)
{
	// Your code that runs when Key Input happens (check GLFW Keyboard Input tutorials)|(it only calls when you play in world, because its Player's function)

	return original(self, window, world, key, scancode, action, mods);
}

$hook(void, StateIntro, init, StateManager& s)
{
	original(self, s);

	// initialize opengl stuff
	glewExperimental = true;
	glewInit();
	glfwInit();
}