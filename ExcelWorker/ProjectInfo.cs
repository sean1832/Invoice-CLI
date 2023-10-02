using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ExcelWorker
{
    internal class ProjectInfo
    {
        public string Title { get; private set; }
        public string Description { get; private set; }
        public string Author { get; private set; }
        public string Copyright { get; private set; }
        public Version Version { get; private set; }

        public ProjectInfo(Assembly assembly)
        {
            if (assembly == null)
            {
                throw new ArgumentNullException(nameof(assembly));
            }

            Title = GetAttribute<AssemblyTitleAttribute>(assembly)?.Title;
            Description = GetAttribute<AssemblyDescriptionAttribute>(assembly)?.Description;
            Author = GetAttribute<AssemblyCompanyAttribute>(assembly)?.Company;
            Copyright = GetAttribute<AssemblyCopyrightAttribute>(assembly)?.Copyright;
            Version = assembly.GetName().Version;
        }

        private T GetAttribute<T>(Assembly assembly) where T : Attribute
        {
            return (T)Attribute.GetCustomAttribute(assembly, typeof(T));
        }
    }
}
