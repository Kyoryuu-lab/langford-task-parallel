<h1>langford-task-parallel</h1>
<p>Langford Sequences L(2, n): Dynamic Tasking and Work-Stealing with tbb::task_group</p>

<h2>Project description</h2>
<p>This project parallelizes recursive backtracking by decomposing the call tree into dynamic parallel tasks:</p>
<ol>
  <li>Replaces recursive call branches with lightweight tasks scheduled via <code>tbb::task_group</code>.</li>
  <li>Leverages TBB work-stealing scheduler to balance dynamically branching workloads across CPU threads.</li>
  <li>
    <dl>
      <dt>Implements recursive cutoff threshold (<code>cutoff_k = n - 3</code>):</dt>
      <dd>Tasks split concurrently at high levels of the search tree.</dd>
      <dd>Sub-trees below cutoff depth revert to fast sequential execution to prevent scheduler overhead.</dd>
    </dl>
  </li>
  <li>Aggregates solution counts thread-safely using <code>std::atomic&lt;uint64_t&gt;</code> and measures Speedup.</li>
</ol>

<h2>Project files</h2>
<table>
  <tr>
    <th colspan="2">langford-task-parallel</th>
  </tr>
  <tr>
    <td>CMakeLists.txt</td>
    <td>CMake build linking Intel oneAPI TBB</td>
  </tr>
  <tr>
    <td>main.cpp</td>
    <td>Parallel implementation using <code>tbb::task_group</code> with depth thresholding</td>
  </tr>
  <tr>
    <td>README.md</td>
    <td>Task-parallelism documentation and scaling metrics</td>
  </tr>
</table>

<h2>Installation and launch</h2>
<ol>
  <b>
    <li>
      <dl>
        <dt>Clone repository</dt>
        <dd>
          <code>git clone https://github.com/Kyoryuu-lab/langford-task-parallel.git</code>
        </dd>
        <dd>
          <code>cd langford-task-parallel</code>
        </dd>
      </dl>
    </li>
  </b>
  <b>
    <li>
      <dl>
        <dt>Build executable</dt>
        <dd>
          <code>cmake -B build -DCMAKE_BUILD_TYPE=Release</code>
        </dd>
        <dd>
          <code>cmake --build build --config Release</code>
        </dd>
      </dl>
    </li>
  </b>
  <b>
    <li>
      <dl>
        <dt>Run task benchmark</dt>
        <dd>
          <code>./build/main</code>
        </dd>
      </dl>
    </li>
  </b>
</ol>

<h2>Example of console output</h2>
<samp>
  <p>
    ====================================================<br />
    PVA Aufgabe 4: Langford V1 (tbb::task_group)<br />
    n = 12 | Threads: 15<br />
    ====================================================<br />
    Baseline sequentiell:    10820.40 ms (1079232 Loesungen)<br />
    Variante 1 (task_group):  1195.62 ms (1079232 Loesungen)<br />
    Validierung:             ERFOLGREICH (100% Match)<br />
    Speedup S:               9.05x<br />
    Effizienz E:             60.3 %<br />
    ====================================================
  </p>
</samp>
