#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(255);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	int count = 0;
	for (int x = -300; x <= 300; x += 600) {

		for (int y = -300; y <= 300; y += 600) {

			ofPushMatrix();
			ofTranslate(x, y, 0);
			ofRotateY(count++ * 45 + ofGetFrameNum());

			for (int r = 60; r <= 90; r += 30) {

				int v_span = 1;
				int u_span = 60;

				ofMesh face, line;
				line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
				for (int v = 0; v <= 360; v += v_span) {

					bool flag = true;
					int u_start = ofMap(ofNoise(r * cos(v * DEG_TO_RAD)  * 0.008, r * sin(v * DEG_TO_RAD) * 0.008, ofGetFrameNum() * 0.0035), 0, 1, -360, 360);
					int next_u = ofMap(ofNoise(r * cos((v + v_span) * DEG_TO_RAD) * 0.008, r * sin((v + v_span) * DEG_TO_RAD) * 0.008, ofGetFrameNum() * 0.0035), 0, 1, -360, 360);
					for (int u = u_start; u < u_start + 360; u += u_span) {

						if (flag) {

							face.addVertex(this->make_point(200, r, u, v));
							face.addVertex(this->make_point(200, r, u + u_span, v));
							face.addVertex(this->make_point(200, r, next_u + u_span, v + v_span));
							face.addVertex(this->make_point(200, r, next_u, v + v_span));

							line.addVertex(this->make_point(200, r, u, v));
							line.addVertex(this->make_point(200, r, u + u_span, v));
							line.addVertex(this->make_point(200, r, next_u + u_span, v + v_span));
							line.addVertex(this->make_point(200, r, next_u, v + v_span));

							ofColor color;
							auto hue = (count * 64 + r * 4 + (int)ofMap(v, 0, 360, 0, 255)) % 255;
							color.setHsb(hue, 180, 255);

							face.addColor(color);
							face.addColor(color);
							face.addColor(color);
							face.addColor(color);

							face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 3);
							face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

							line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 4);
							line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 3);
						}

						next_u += u_span;
						flag = !flag;
					}
				}

				face.drawFaces();
				line.drawWireframe();
			}

			ofPopMatrix();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}