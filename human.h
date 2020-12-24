#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "myFrustum.h"
#include "drawFrustum.h"
#include "transformation.h"
#include "light.h"

#include <unordered_map>
#include <string>

const glm::vec3 green(0.0f, 1.0f, 0.0f);
const glm::vec3 red(1.0f, 0.0f, 0.0f);
const glm::vec3 blue(0.0f, 0.0f, 1.0f);
const glm::vec3 black(0.0f, 0.0f, 0.0f);
const glm::vec3 purple(0.55f, 0.05f, 0.89f);
const glm::vec3 white(1.0f, 1.0f, 1.0f);
const glm::vec3 test(0.5f, 0.5f, 0.5f);

static std::unordered_map<std::string, int> number{
	{"upperbody", 0},
	{"head", 1},
	{"leftshoulder", 2},
	{"left_upperarm", 3},
	{"left_lowerarm", 4},
	{"rightshoulder", 5},
	{"right_upperarm", 6},
	{"right_lowerarm", 7},
	{"lowerbody", 8},
	{"leftupperleg", 9},
	{"leftlowerleg", 10},
	{"rightupperleg", 11},
	{"rightlowerleg", 12}
};

class Human {
private:
	Frustum upperbody;
	Frustum head;
	Frustum leftshoulder;
	Frustum left_upperarm;
	Frustum left_lowerarm;
	Frustum rightshoulder;
	Frustum right_upperarm;
	Frustum right_lowerarm;
	Frustum lowerbody;
	Frustum leftupperleg;
	Frustum leftlowerleg;
	Frustum rightupperleg;
	Frustum rightlowerleg;

	std::vector<treenode> nodes;

public:
	Human(float base = 1.0f):
		upperbody(3.0f * base, 3.0f * base, 4.0f * base, red),
		head(1.0f * base, 1.0f * base, 1.0f * base, green),
		lowerbody(3.0f * base, 3.0f * base, 2.0f * base, purple),

		leftupperleg(1.0f * base, 1.0f * base, 2.0f * base, red),
		leftlowerleg(1.0f * base, 1.0f * base, 3.0f * base, blue),

		rightupperleg(1.0f * base, 1.0f * base, 2.0f * base, red),
		rightlowerleg(1.0f * base, 1.0f * base, 3.0f * base, blue),

		leftshoulder(2.0f * base, 2.0f * base, 1.0f * base, blue),
		rightshoulder(2.0f * base, 2.0f * base, 1.0f * base, blue),

		left_upperarm(1.0f * base, 1.0f * base, 2.0f * base, black),
		right_upperarm(1.0f * base, 1.0f * base, 2.0f * base, black),
		left_lowerarm(1.0f * base, 1.0f * base, 2.0f * base, white),
		right_lowerarm(1.0f * base, 1.0f * base, 2.0f * base, white),

		nodes(13)

	{
		node_object();
		node_relation();
		node_related_loc(base);

	}

public:
	// 绘制
	void draw(Transform* transform, Light * light, const Camera& camera) {
		traverse(&nodes[number["upperbody"]], transform, light, camera);
	}
	// 摆动
	void rotate_rightuppperarm(float angle) {
		auto model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		nodes[number["right_upperarm"]].localModel = model * nodes[number["right_upperarm"]].localModel;
	}
	//
	void rotate_rightupperleg(float angle) {
		auto model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		nodes[number["rightupperleg"]].localModel = model * nodes[number["rightupperleg"]].localModel;
	}

private:
	// 为每个节点匹配合适的身体部分
	void node_object() {
		nodes[number["upperbody"]].object = &upperbody;
		nodes[number["lowerbody"]].object = &lowerbody;
		nodes[number["leftupperleg"]].object = &leftupperleg;
		nodes[number["leftlowerleg"]].object = &leftlowerleg;
		nodes[number["rightupperleg"]].object = &rightupperleg;
		nodes[number["rightlowerleg"]].object = &rightlowerleg;
		nodes[number["rightshoulder"]].object = &rightshoulder;
		nodes[number["right_upperarm"]].object = &right_upperarm;
		nodes[number["right_lowerarm"]].object = &right_lowerarm;
		nodes[number["head"]].object = &head;
		nodes[number["leftshoulder"]].object = &leftshoulder;
		nodes[number["left_upperarm"]].object = &left_upperarm;
		nodes[number["left_lowerarm"]].object = &left_lowerarm;
	}


	void node_relation() {
		nodes[number["upperbody"]].child = &nodes[number["lowerbody"]];

		nodes[number["lowerbody"]].child = &nodes[number["leftupperleg"]];
		nodes[number["lowerbody"]].sibling = &nodes[number["leftshoulder"]];
		nodes[number["leftupperleg"]].child = &nodes[number["leftlowerleg"]];
		nodes[number["leftupperleg"]].sibling = &nodes[number["rightupperleg"]];

		nodes[number["rightupperleg"]].child = &nodes[number["rightlowerleg"]];
		
		nodes[number["leftshoulder"]].child = &nodes[number["left_upperarm"]];
		nodes[number["leftshoulder"]].sibling = &nodes[number["head"]];
		nodes[number["left_upperarm"]].child = &nodes[number["left_lowerarm"]];

		nodes[number["head"]].sibling = &nodes[number["rightshoulder"]];

		nodes[number["rightshoulder"]].child = &nodes[number["right_upperarm"]];
		nodes[number["right_upperarm"]].child = &nodes[number["right_lowerarm"]];
	}

	// 确定相对位置
	void node_related_loc(float base) {
		glm::vec3 phead(0.0f, 2.5f * base, 0.0f);
		glm::vec3 prightshoulder(2.5f * base, 1.5f * base, 0.0f);
		glm::vec3 pleftshoulder(-2.5f * base, 1.5f * base, 0.0f);
		glm::vec3 prightupperarm(3.0f * base, 0.0f, 0.0f);
		glm::vec3 pleftupperarm(-3.0f * base, 0.0f, 0.0f);
		glm::vec3 prightlowerarm(3.0f * base, -2.0f * base, 0.0f);
		glm::vec3 pleftlowerarm(-3.0f * base, -2.0f * base, 0.0f);
		glm::vec3 plowerbody(0.0f, -3.0f * base, 0.0f);
		glm::vec3 prightupperleg(1.0f * base, -5.0f * base, 0.0f);
		glm::vec3 pleftupperleg(-1.0f * base, -5.0f * base, 0.0f);
		glm::vec3 prightlowerleg(1.0f * base, -7.5f * base, 0.0f);
		glm::vec3 pleftlowerleg(-1.0f * base, -7.5f * base, 0.0f);
		
		glm::mat4 model(1.0f);
		nodes[number["head"]].localModel = glm::translate(model, phead);
		nodes[number["rightshoulder"]].localModel = glm::translate(model, prightshoulder);
		nodes[number["leftshoulder"]].localModel = glm::translate(model, pleftshoulder);
		nodes[number["right_upperarm"]].localModel = glm::translate(model, prightupperarm - prightshoulder);
		nodes[number["left_upperarm"]].localModel = glm::translate(model, pleftupperarm - pleftshoulder);
		nodes[number["right_lowerarm"]].localModel = glm::translate(model, prightlowerarm - prightupperarm);
		nodes[number["left_lowerarm"]].localModel = glm::translate(model, pleftlowerarm - pleftupperarm);
		nodes[number["lowerbody"]].localModel = glm::translate(model, plowerbody);
		nodes[number["rightupperleg"]].localModel = glm::translate(model, prightupperleg - plowerbody);
		nodes[number["leftupperleg"]].localModel = glm::translate(model, pleftupperleg - plowerbody);
		nodes[number["rightlowerleg"]].localModel = glm::translate(model, prightlowerleg - prightupperleg);
		nodes[number["leftlowerleg"]].localModel = glm::translate(model, pleftlowerleg - pleftupperleg);
	}

};

#endif // !HUMAN_H
