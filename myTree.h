#pragma once
#ifndef MYTREE_H
#define MYTREE_H

#include "myFrustum.h"
#include "drawFrustum.h"

typedef glm::vec3 Position;

class MyTree {
public:
	Frustum stem;
	Frustum leafTop;
	Frustum leafLower;
	
	Position pStem;
	Position pLeafTop;
	Position pLeafLower;

	std::vector<treenode> nodes;

	MyTree() :
		stem(0.25f, 0.25f, 0.5f, glm::vec3(0.64f, 0.42f, 0.44f)), // ��ʼ����״
		leafTop(0.01f, 0.5f, 0.5f, glm::vec3(0, 0.9f, 0)),
		leafLower(0.25f, 0.5f, 0.25f, glm::vec3(0, 0.9f, 0)),
		nodes(3)
	{
		// ��������**��ʼ���**λ�ã�ע�ⶼ�Ǵ�ԭ�㿪ʼ����״�Ѿ����ú�
		pStem = glm::vec3(0.0f, 0.5 * stem.getHeight(), 0.0f);
		pLeafLower = glm::vec3(0.0f, 0.5 * stem.getHeight() +  0.5 * leafLower.getHeight(), 0.0f);
		pLeafTop = glm::vec3(0.0f, 0.5 * leafLower.getHeight() +  0.5 * leafTop.getHeight(), 0.0f);

		// �������ͽṹ

		// ���û����任���󣬳�ʼ������
		// stem
		nodes[0].localModel = glm::translate(nodes[0].localModel, pStem);
		nodes[0].object = &stem;
		nodes[0].child = &nodes[1];
		// lowerleaf
		nodes[1].localModel = glm::translate(nodes[1].localModel, pLeafLower);
		nodes[1].object = &leafLower;
		nodes[1].child = &nodes[2];
		// uppperleaf
		nodes[2].localModel = glm::translate(nodes[2].localModel, pLeafTop);
		nodes[2].object = &leafTop;

	}

	void draw(Transform* transform, Light* light, const Camera& camera) {
		traverse(&nodes[0], transform, light, camera);
	}


	void translate(glm::vec3 direction) {
		nodes[0].localModel = glm::translate(nodes[0].localModel, direction);
	}
	
};


#endif // !MYTREE_H

