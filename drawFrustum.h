#pragma once
#ifndef DRAWFRUSTUM_H
#define DRAWFRUSTUM_H

#include <learnopengl/camera.h>

#include "myFrustum.h"
#include "light.h"
#include "transformation.h"

#include <stack>


static std::stack < glm::mat4> stackModel;
// ---------------------------------------------
// -----------------�������νṹ----------------
struct treenode {
	// �������ͼ�νڵ�
	glm::mat4 localModel;
	Frustum* object;
	treenode* sibling;
	treenode* child;

	treenode(): localModel(1.0f), object(nullptr), sibling(nullptr), child(nullptr) {}
	treenode(Frustum* object, treenode* sibling, treenode* child, glm::mat4 localModel) {
		this->object = object;
		this->sibling = sibling;
		this->child = child;
		this->localModel = localModel;
	}
};

// -----------------�������νṹ----------------
void traverse(treenode*node, Transform * transform, Light* light, const Camera& camera) {
	// �Ӹ��ڵ㿪ʼ����
	if (!node) return;
	// ����֮ǰ�ľ���
	stackModel.push(transform->model);
	transform->model = transform->model * node->localModel;
	// �ı�֮����Ҫ��Ⱦ
	node->object->draw(transform->model, transform->view, transform->projection, *light, camera);

	// ֧��Ľڵ���Ҫ����
	if (node->child) traverse(node->child, transform, light, camera);
	// ͬһ���Ľڵ�ֻ��Ҫԭ���ľ���
	transform->model = stackModel.top();
	stackModel.pop();
	if (node->sibling) traverse(node->sibling, transform, light, camera);
}



#endif // !DRAWFRUSTUM_H

