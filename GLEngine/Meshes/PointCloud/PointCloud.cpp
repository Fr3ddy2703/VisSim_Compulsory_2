#include "pch.h"
#include "PointCloud.h"
#include "../../Shaders/Shader.h"

//void PointCloud::CreatePointCloudFromFile(const char* _fileDirectory, float _scaleFactor)
//{
//	//laszip_POINTER laszip_reader;
//	//if (laszip_create(&laszip_reader))
//	//{
//	//	std::cout << "Failed to create laszip reader\n";
//	//}
//
//	//laszip_BOOL is_compressed = true;
//	//if (laszip_open_reader(laszip_reader, _fileDirectory, &is_compressed))
//	//{
//	//	std::cout << "Failed to open laszip reader\n";
//	//}
//
//	//laszip_header* header;
//	//if (laszip_get_header_pointer(laszip_reader, &header))
//	//{
//	//	std::cout << "Failed to get header pointer\n";
//	//}
//
//	//laszip_point* point;
//	//if (laszip_get_point_pointer(laszip_reader, &point))
//	//{
//	//	std::cout << "Failed to get point pointer\n";
//	//}
//
// //   laszip_I64 numTotalPoints = (header->number_of_point_records ? header->number_of_point_records : header->extended_number_of_point_records);
//
//	//std::cout << "Number of points: " << numTotalPoints << "\n";
//
//	//for (int i = 0; i < numTotalPoints; i++)
//	//{
//	//	if (laszip_read_point(laszip_reader))
//	//		std::cout << "Failed to read point\n";
//	//	float x = (float)(point->X * header->x_scale_factor + header->x_offset) * _scaleFactor;
//	//	float y = (float)(point->Y * header->y_scale_factor + header->y_offset) * _scaleFactor;
//	//	float z = (float)(point->Z * header->z_scale_factor + header->z_offset) * _scaleFactor;
//
//	//	glm::vec3 colorf = glm::vec3(0);
//	//	glm::vec3 vpos = glm::vec3(x, y, z);
//
//	//	Vertices.push_back(Vertex(vpos, colorf));
//	//	Indices.push_back(Triangle(i, i, i));
//	//}
//	//if (laszip_close_reader(laszip_reader))
//	//{
//	//	std::cout << "Failed to close laszip reader\n";
//	//}
//	//if (laszip_destroy(laszip_reader))
//	//{
//	//	std::cout << "Failed to destroy laszip reader\n";
//	//}
//}

void PointCloud::CreatePointCloudFromFile(const char* _fileDirectory, float _scaleFactor)
{
// Open the file
    std::ifstream file(_fileDirectory);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << _fileDirectory << "\n";
        return; // Exit if the file cannot be opened
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        float x, y, z;

        // Try to parse the x, y, z coordinates
        if (!(iss >> x >> y >> z))
        {
            std::cout << "Failed to parse line: " << line << "\n";
            continue; // Skip this line if parsing fails, but don't break the loop
        }

        // Scale the coordinates and create a vertex
        glm::vec3 colorf = glm::vec3(0); // Placeholder color (set to black)
        glm::vec3 vpos = glm::vec3(x, y, z) * _scaleFactor;

        // Add vertex and triangle
        Vertices.push_back(Vertex(vpos, colorf));
        Indices.push_back(Triangle(Vertices.size() - 1, Vertices.size() - 1, Vertices.size() - 1));
    }

    // Bind the buffer (ensure this method is correctly implemented)
    BindBuffer();

    std::cout << "Finished reading point cloud from file\n";
}

void PointCloud::Draw()
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, size);
    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, Indices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}

void PointCloud::BindBuffer()
{
	// VAO
    glGenBuffers(1, &VBO);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(Triangle), Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
