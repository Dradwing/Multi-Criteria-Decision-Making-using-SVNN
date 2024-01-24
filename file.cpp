#include <bits/stdc++.h>
#include <glpk.h>
#include "matplotlibcpp.h"
#include <json/json.h>

using namespace std;
namespace plt = matplotlibcpp;

struct Particle {
  vector<double> position; // Particle's position in the search space
  vector<double> velocity; // Particle's velocity
  double fitness; // Particle's fitness value
  vector<double> best_position; // Best position found by the particle
  double best_fitness; // Best fitness value found by the particle
};

class PSO {
public:
  PSO(int num_dimensions, vector<vector<double>> input_ranges, vector<double> input_coefficients, vector<vector<vector<double>>> &history, vector<vector<double>> &final_results);
  vector<double> optimize(vector<vector<vector<double>>> &history, vector<vector<double>> &final_results);
  double fitness_function(const vector<double>& position);
  void visualize2D(vector<vector<vector<double>>> &history);

private:
  int num_particles = 500;
  int num_dimensions;
  int max_iterations = 300;
  double cognitive_weight = 2.0;
  double social_weight = 1.5;
  double inertia_weight = 0.7;
  vector<vector<double>> range;
  vector<double> coefficients;
  vector<Particle> particles;
  vector<double> global_best_position;
  double global_best_fitness;

  void initialize_particles(vector<vector<vector<double>>> &history, vector<vector<double>> &final_results);
  void update_particle(Particle& particle);
};

PSO::PSO(int num_dimensions, vector<vector<double>> input_ranges, vector<double> input_coefficients, vector<vector<vector<double>>> &history, vector<vector<double>> &final_results)
  : num_dimensions(num_dimensions), range(input_ranges), coefficients(input_coefficients) {
  particles.resize(num_particles);
  global_best_fitness = numeric_limits<double>::min();

  initialize_particles(history, final_results);
}

void PSO::initialize_particles(vector<vector<vector<double>>> &history, vector<vector<double>> &final_results) {

  vector<vector<double>> curr_iteration;
  for (int i = 0; i < num_particles; ++i) {
    particles[i].position.resize(num_dimensions);
    particles[i].velocity.resize(num_dimensions);

    for (int j = 0; j < num_dimensions; ++j) {
      particles[i].position[j] = range[j][0];
      // NEET TO MAKE OPTIMAL
      particles[i].velocity[j] = 0.1;
    }

    particles[i].fitness = fitness_function(particles[i].position);


    vector<double> temp(7);
    temp[0] = particles[i].position[0];
    temp[1] = particles[i].position[1];
    temp[2] = particles[i].position[2];
    temp[3] = particles[i].position[3];
    temp[4] = particles[i].position[4];
    temp[5] = particles[i].position[5];
    temp[6] = particles[i].fitness;

    curr_iteration.push_back(temp);

    if (particles[i].fitness > particles[i].best_fitness) {
      particles[i].best_position = particles[i].position;
      particles[i].best_fitness = particles[i].fitness;
    }

    if (particles[i].best_fitness > global_best_fitness) {
      global_best_fitness = particles[i].best_fitness;
      global_best_position = particles[i].best_position;
    }
  }
  history.push_back(curr_iteration);
  vector<double> curr_best_temp;
  curr_best_temp = global_best_position;
  curr_best_temp.push_back(global_best_fitness);
  final_results.push_back(curr_best_temp);
}

void PSO::update_particle(Particle& particle) {
  for (int i = 0; i < num_dimensions; ++i) {
    double cognitive_random = static_cast<double>(rand()) / RAND_MAX;
    double social_random = static_cast<double>(rand()) / RAND_MAX;

// Calculate cognitive and social components
    double cognitive_component = cognitive_weight * cognitive_random * (particle.best_position[i] - particle.position[i]);
    double social_component = social_weight * social_random * (global_best_position[i] - particle.position[i]);

// Update velocity
    particle.velocity[i] = inertia_weight * particle.velocity[i] + cognitive_component + social_component;

// Update position
    particle.position[i] += particle.velocity[i];

  }

  // Apply the additional constraints
  for (int i = 0; i < num_dimensions; i++)
  {
    if (particle.position[i] < range[i][0])particle.position[i] = range[i][0];
    if (particle.position[i] > range[i][1])particle.position[i] = range[i][1];
  }

  double new_fitness = fitness_function(particle.position);

  // summation constraint: sum of all = 1
  double sum = particle.position[0] + particle.position[1] + particle.position[2] + particle.position[3] + particle.position[4] + particle.position[5];

  if (sum > 1)
    new_fitness = max((double)0, new_fitness - abs(1.00 - sum) * 20.3);


  if (new_fitness > particle.best_fitness) {
    particle.best_fitness = new_fitness;
    particle.best_position = particle.position;

    if (particle.best_fitness > global_best_fitness) {
      global_best_fitness = particle.best_fitness;
      global_best_position = particle.best_position;
    }
  }
  particle.fitness = new_fitness;
}

vector<double> PSO::optimize(vector<vector<vector<double>>> &history, vector<vector<double>> &final_results) {
  for (int iteration = 0; iteration < max_iterations; ++iteration) {
    vector<vector<double>> curr_iteration;
    for (int i = 0; i < num_particles; ++i) {
      update_particle(particles[i]);

      vector<double> temp(7);
      temp[0] = particles[i].position[0];
      temp[1] = particles[i].position[1];
      temp[2] = particles[i].position[2];
      temp[3] = particles[i].position[3];
      temp[4] = particles[i].position[4];
      temp[5] = particles[i].position[5];
      temp[6] = particles[i].fitness;

      curr_iteration.push_back(temp);
    }
    history.push_back(curr_iteration);
    vector<double> curr_best_temp;
    curr_best_temp = global_best_position;
    curr_best_temp.push_back(global_best_fitness);
    final_results.push_back(curr_best_temp);

  }

  vector<double> ans;
  for (int i = 0; i < num_dimensions; ++i) {
    ans.push_back(global_best_position[i]);
  }

  return ans;
}

double PSO::fitness_function(const vector<double>& position) {

  double result = 0;

  for (int i = 0; i < coefficients.size(); i++)
    result += coefficients[i] * position[i];

  return result;
};

void PSO::visualize2D(vector<vector<vector<double>>> &history)
{
  plt::title("PSO Algorithm Iterations");
  plt::xlabel("X");
  plt::ylabel("Y");

  for (int i = 0; i < history.size(); i++)
  {
    std::vector<double> x_vals, y_vals;
    for (int j = 0; j < history[i].size(); ++j) {
      x_vals.push_back(history[i][j][0]);
      y_vals.push_back(history[i][j][3]);
    }

    plt::scatter(x_vals, y_vals);
    plt::pause(0.01);
    plt::clf();
  }
}


class LPP {
public:
  vector<double> coefficients;
  vector<vector<double>> ranges;
  glp_prob* lp;

  // Constructor
  LPP(vector<double> a, vector<vector<double>> b) {
    coefficients = a;
    ranges = b;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "Linear Programming Problem");
    glp_set_obj_dir(lp, GLP_MAX); // Maximizing

    glp_add_rows(lp, 1); // 1 constraint
    glp_set_row_name(lp, 1, "Constraint1");
    glp_set_row_bnds(lp, 1, GLP_FX, 1.0, 1.0);

    glp_add_cols(lp, static_cast<int>(coefficients.size())); // Variables

    int ia[1 + 1000];
    int ja[1 + 1000];
    double ar[1 + 1000];
    int idx = 1; // Matrix index

    for (int i = 1; i <= static_cast<int>(coefficients.size()); i++) {
      glp_set_col_name(lp, i, ("x" + to_string(i)).c_str());
      glp_set_col_bnds(lp, i, GLP_DB, ranges[i - 1][0], ranges[i - 1][1]);
      glp_set_obj_coef(lp, i, coefficients[i - 1]);

      ia[idx] = 1;
      ja[idx] = i;
      ar[idx] = 1.0;
      idx++;
    }

    glp_load_matrix(lp, idx - 1, ia, ja, ar);
  }

  // Destructor
  ~LPP() {
    glp_delete_prob(lp);
  }

  vector<double> solveLppBySimplex() {
    vector<double> ans;
    glp_smcp params;
    glp_init_smcp(&params);
    params.msg_lev = GLP_MSG_OFF; // Turn off GLPK output

    int ret = glp_simplex(lp, &params);

    if (ret == 0) {
      double z = glp_get_obj_val(lp);
      cout << "Simplex Method - Objective Value: " << z << endl;

      int iterations = glp_get_it_cnt(lp); // Corrected line
      cout << "Simplex Method - Iterations: " << iterations << endl;

      for (int i = 1; i <= static_cast<int>(coefficients.size()); i++) {
        double xi = glp_get_col_prim(lp, i);
        ans.push_back(xi);
      }
    }

    return ans;
  }

  vector<double> solveLppByInteriorPoint() {
    vector<double> ans;
    glp_iptcp params;
    glp_init_iptcp(&params);
    params.msg_lev = GLP_MSG_OFF; // Turn off GLPK output

    int ret = glp_interior(lp, &params);

    if (ret == 0) {
      double z = glp_get_obj_val(lp);

      for (int i = 1; i <= static_cast<int>(coefficients.size()); i++) {
        double xi = glp_ipt_col_prim(lp, i);
        ans.push_back(xi);
      }
    }

    return ans;
  }


};

int main() {

  // number of alternatives
  // number of factors
  // number of linguistic terms
  // linguistic term with their values
  // number of experts
  // input of each expert
  // value of alpha for score vs accuracy
  // input weight of each factor

  int num_alternatives, num_factors, num_linguistic_terms;

  cout << "Input the number of alternatives: " << endl;
  cin >> num_alternatives;

  cout << "Input the number of dependent factors: " << endl;
  cin >> num_factors;

  cout << "Input the number of linguistic terms: " << endl;
  cin >> num_linguistic_terms;

  unordered_map<string, vector<double>> m;

  for (int i = 0; i < num_linguistic_terms; i++) {
    string lingusitic;
    double a, b, c;
    cout << "Input linguistic term: " << endl;
    cin >> lingusitic;
    cout << "Input respective values for truth, indeterminacy and falsity membership: " << endl;
    cin >> a >> b >> c;

    m[lingusitic] = {a, b, c};
  }

  int num_experts;

  cout << "\nInput the number of experts: " << endl;
  cin >> num_experts;

  vector<vector<vector<string>>> decision_matrix(num_experts, vector<vector<string>>(num_alternatives, vector<string> (num_factors)));

  for (int k = 0; k < num_experts; k++) {
    cout << "For expert " << k + 1 << endl;

    for (int i = 0; i < num_alternatives; i++) {
      cout << "\nFor alternative " << i + 1 << " enter linguistic terms of all factors: " << endl;
      for (int j = 0; j < num_factors; j++)cin >> decision_matrix[k][i][j];
    }
    cout << endl;
  }

  // calculating composite precision matrix

  double alpha;
  cout << "\nInput the value of alpha : ";
  cin >> alpha;

  vector<vector<vector<double>>> composite_precison_matrix(num_experts, vector<vector<double>>(num_alternatives, vector<double> (num_factors)));

  for (int i = 0; i < num_experts; i++) {
    for (int j = 0; j < num_alternatives; j++) {
      for (int k = 0; k < num_factors; k++) {
        vector<double> curr = m[decision_matrix[i][j][k]];
        composite_precison_matrix[i][j][k] = (double(0.5)) * alpha * (1.00 + curr[0] - curr[2]) + (1.00 / 3.000) * (1.00 - alpha) * (2 + curr[0] - curr[1] - curr[2]);
      }
    }
  }

  // calculating average matrix to calculate weights of each expert

  vector<vector<double>> mean_matrix(num_alternatives, vector<double>(num_factors));


  for (int i = 0; i < num_alternatives; i++)
    for (int j = 0; j < num_factors; j++)
    {
      double total = 0.00;
      for (int k = 0; k < num_experts; k++)total += composite_precison_matrix[k][i][j];
      mean_matrix[i][j] = total / ((double)num_experts);
    }


  // calculating collective correlation coefficient for each expert

  vector<double> collective_correlation_coefficient(num_experts);

  double net_coefficient = 0.00;

  for (int k = 0; k < num_experts; k++)
  {
    double ans = 0;
    for (int i = 0; i < num_alternatives; i++) {

      double product = 0.0, hs_sq_sum = 0.0, hybrid_sq_sum = 0;

      for (int j = 0; j < num_factors; j++) {
        product += composite_precison_matrix[k][i][j] * mean_matrix[i][j];
        hybrid_sq_sum += mean_matrix[i][j] * mean_matrix[i][j];
        hs_sq_sum += composite_precison_matrix[k][i][j] * composite_precison_matrix[k][i][j];
      }

      hs_sq_sum = sqrt(hs_sq_sum);
      hybrid_sq_sum = sqrt(hybrid_sq_sum);

      ans += product / (hybrid_sq_sum * hs_sq_sum);
    }
    collective_correlation_coefficient[k] = ans;

    net_coefficient += ans;
  }

  // calculating decison's makers/ experts weights

  vector<double> decision_maker_weights(num_experts);



  for (int k = 0; k < num_experts; k++)
    decision_maker_weights[k] = collective_correlation_coefficient[k] / net_coefficient;


  // calculating unified composite precision matrix;

  vector<vector<double>> unified_composite_precision_matrix(num_alternatives, vector<double>(num_factors));

  for (int i = 0; i < num_alternatives; i++)
  { for (int j = 0; j < num_factors; j++)
    {
      double total = 0.00;
      for (int k = 0; k < num_experts; k++)total += composite_precison_matrix[k][i][j] * decision_maker_weights[k];
      unified_composite_precision_matrix[i][j] = total;
    }
  }

  // calculating single weight for each column for each row;

  vector<double> coefficients(num_factors, 0.00);
  vector<vector<double>> ranges(num_factors, vector<double> (2));

  for (int j = 0; j < num_factors; j++) {
    for (int i = 0; i < num_alternatives; i++)coefficients[j] += unified_composite_precision_matrix[i][j];
  }

  for (int j = 0; j < num_factors; j++)
  {
    cout << "Enter the weight range for " << j + 1 << " factor: " << endl;
    cin >> ranges[j][0] >> ranges[j][1];
  }

  vector<double> final_weight;

  //Solve By Simplex or Interior Point Method or PSO


  int num_trials = 10;
  double total_time = 0.0;

  for (int i = 0; i < num_trials; ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();

    LPP current_lpp(coefficients, ranges);
    final_weight = current_lpp.solveLppBySimplex();
    //final_weight = current_lpp.solveLppByInteriorPoint();

    //Solve By PSO

    /*vector<vector<vector<double>>> history;
    vector<vector<double>> final_results;
    srand(time(nullptr));
    PSO pso(num_factors, ranges, coefficients, history, final_results);
    final_weight = pso.optimize(history, final_results);
    */

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    total_time += duration.count();
  }

  double average_time = total_time / num_trials;
  std::cout << "Average Execution Time: " << average_time << " microsseconds" << std::endl;



  // saving the results of PSO iterations to csv file:

  /*ofstream csvFile("pso_results.csv");

  // Check if the file is open
  if (csvFile.is_open()) {
  csvFile << "Iteration,x1,x2,x3,x4,x5,x6,Objective Value" << std::endl;

  for(int iteration=1;iteration<=final_results.size();iteration++)
  {
  csvFile<<iteration<<","<<final_results[iteration-1][0]<<","<<final_results[iteration-1][1]<<","<<final_results[iteration-1][2]<<","<<final_results[iteration-1][3]<<","<<final_results[iteration-1][4]<<","<<final_results[iteration-1][5]<<","<<final_results[iteration-1][6]<<endl;
  }

  // Close the file
  csvFile.close();
  std::cout << "3D vector data has been saved to 3d_points.csv" << std::endl;
  } else {
  std::cerr << "Error: Could not open the file for writing." << std::endl;
  }


  // Saving the data in json file

  Json::Value root;
  for (const auto& iteration : history) {
  Json::Value iterationData;
  for (const auto& particle : iteration) {
   Json::Value particleData;
   for (const double& coord : particle) {
       particleData.append(coord);
   }
   iterationData.append(particleData);
  }
  root.append(iterationData);
  }

  Json::StreamWriterBuilder writer;
  std::string dataStr = Json::writeString(writer, root);
  std::ofstream outFile("data.json");
  if (outFile.is_open()) {
  outFile << dataStr;
  outFile.close();
  std::cout << "JSON data saved to data.json" << std::endl;
  } else {
  std::cerr << "Error saving JSON data to file" << std::endl;
  return 1;
  }

  //  Visualizing the PSO

  // pso.visualize2D(history);
  system("python plot3D.py");
  */


  cout << "\nFinal Weights for each factor: ";
  for (int i = 0; i < final_weight.size(); i++)cout << final_weight[i] << " ";

  // calculating final values for each alternative

  vector<pair<double, int>> final_hybrid(num_alternatives);


  for (int i = 0; i < num_alternatives; i++) {
    double total = 0.00;

    for (int j = 0; j < num_factors; j++)total += final_weight[j] * unified_composite_precision_matrix[i][j];
    pair<double, int> p;
    p.second = i + 1;
    p.first = total;
    final_hybrid[i] = p;
  }

  // sorting the result

  sort(final_hybrid.begin(), final_hybrid.end());
  reverse(final_hybrid.begin(), final_hybrid.end());

  cout << "\nOptimal squence is: ";

  for (int i = 0; i < num_alternatives; i++)cout << final_hybrid[i].second << " ";

}
