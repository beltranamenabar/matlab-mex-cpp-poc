#include "mex.hpp"
#include "mexAdapter.hpp"

class MexFunction : public matlab::mex::Function {

public:

    MexFunction () : matlabPtr(getEngine()) {}

    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

        checkArguments(inputs);

        matlab::data::Array obj(inputs[0]);
        matlab::data::Array data(matlabPtr->getProperty(obj, u"Data"));

        stream << data.getDimensions()[0] << " " << data.getDimensions()[1];

        matlab::data::TypedArray<double> points(matlabPtr->getProperty(data[0], u"Points"));
        matlab::data::TypedArray<uint16_t> triangles(matlabPtr->getProperty(data[0], u"ConnectivityList"));

        matlab::data::Array polygons(matlabPtr->getProperty(obj, u"Polygon"));

        for(int i = 0; i < polygons.getDimensions()[0]; i++) {

            uint16_t p0 = triangles[i][0];
            uint16_t p1 = triangles[i][1];
            uint16_t p2 = triangles[i][2];

            matlab::data::TypedArray<double> polygon = factory.createArray(
                {3,2},
                {
                    points[p0][0], points[p1][0], points[p2][0],
                    points[p0][1], points[p1][1], points[p2][1]
                }
            );
            matlabPtr->setProperty(polygons, i, u"Vertices", polygon);
        }
        // Return modified object
        outputs[0] = obj;
    }

private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr;
    std::ostringstream stream;
    matlab::data::ArrayFactory factory;

    /**
     * @brief Checks if the input arguments are valid.
     *
     * @param inputs The inputs to be checked
     */
    void checkArguments(matlab::mex::ArgumentList inputs) {

        // You only have to pass the object
        if (inputs.size() != 1) {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<matlab::data::Array>({factory.createScalar("Only one input required")}));
        }

        // Check if the object is a meshConverter
        std::vector<matlab::data::Array> args{inputs[0], factory.createCharArray("meshConverter")};
        matlab::data::TypedArray<bool> result = matlabPtr->feval(u"isa", args);
        if (result[0] != true) {
            matlabPtr->feval(u"error", 0,
                             std::vector<matlab::data::Array>({factory.createScalar("Input must be meshConverter object")}));
        }
    }

    /**
     * @brief A simple utility function to display the string on MATLAB
     *
     * @param stream A stringstream object with the msg to be displayed
     */
    void displayOnMATLAB(std::ostringstream &stream) {
        // Pass stream content to MATLAB fprintf function
        matlabPtr->feval(u"fprintf", 0,
                         std::vector<matlab::data::Array>({factory.createScalar(stream.str())}));
        // Clear stream buffer
        stream.str("");
    }
};